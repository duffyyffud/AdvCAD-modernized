# GM3D Independent Viewer - Working Version
# This is the restored working version with rotating axes and interactive center object

import sys
import math
from typing import List, Tuple, Optional
from PyQt5 import QtCore 

try:
    from PyQt5 import QtWidgets, QtCore, QtOpenGL
    from PyQt5.QtCore import Qt
    OPENGL_AVAILABLE = True
    try:
        import OpenGL.GL as gl
        import OpenGL.GLU as glu
        try:
            import OpenGL.GLUT as glut
            _HAS_GLUT = True
        except ImportError:
            _HAS_GLUT = False
    except ImportError:
        OPENGL_AVAILABLE = False
        _HAS_GLUT = False
        print("Warning: OpenGL not available")
except ImportError:
    OPENGL_AVAILABLE = False
    print("Warning: PyQt5 not available")

_LEGACY = hasattr(QtOpenGL, 'QGLWidget')

# Type aliases
Vec3 = Tuple[float, float, float]
Face3 = List[int]

class MeshViewer3D(QtOpenGL.QOpenGLWidget if not _LEGACY else QtOpenGL.QGLWidget):
    """3D mesh viewer with interactive controls and coordinate axes display."""

    def __init__(self, parent=None):
        super().__init__(parent)
        
        # Initialize GLUT if available
        if _HAS_GLUT:
            glut.glutInit()
        
        # Mesh data
        self.vertices: List[Vec3] = []
        self.faces: List[Face3] = []

        # View state (kept for compatibility but not used for object)
        self.rotation_x: float = 20.0
        self.rotation_y: float = -30.0
        self.pan_x: float = 0.0
        self.pan_y: float = 0.0
        self.zoom: float = 1.0  # fit状態を1.0とする相対倍率
        self._fit_dist: float = 6.0  # fit_to_view()が決めるカメラ距離（zoom=1.0の基準）
        self.wireframe_mode: bool = True

        self._last_pos = None
        self._last_button = None

        self.setFocusPolicy(QtCore.Qt.StrongFocus)
        self.setMinimumSize(400, 300)

        # Trackball orientation (w, x, y, z)
        self._q = (1.0, 0.0, 0.0, 0.0)   # identity
        self._trackball_speed = 0.015

        # Projection mode: True = perspective, False = orthographic
        self.perspective_mode: bool = True

    def load_mesh(self, vertices, faces):
        """メッシュ読込：1-based→0-based補正／重心・半径の計算／再描画"""
        print("DEBUG: load_mesh called.")
        vs = [(float(x), float(y), float(z)) for (x, y, z) in vertices]
        fs = [tuple(int(i) for i in f) for f in faces if len(f) >= 3]

        # 1-based → 0-based 自動補正
        if fs:
            mn = min(min(f) for f in fs); mx = max(max(f) for f in fs)
            if mn == 1 and mx <= len(vs):
                fs = [tuple(i - 1 for i in f) for f in fs]

        # 範囲外を落とす
        n = len(vs)
        fs = [f for f in fs if all(0 <= i < n for i in f)]

        self.vertices = vs
        self.faces = fs
        self._recalc_bounds()   
        self.fit_to_view()      
        self._update()

    def _recalc_bounds(self):
        if not self.vertices:
            self._center = (0.0, 0.0, 0.0)
            self._radius = 1.0
            return
        xs = [v[0] for v in self.vertices]
        ys = [v[1] for v in self.vertices]
        zs = [v[2] for v in self.vertices]
        cx = (max(xs) + min(xs)) * 0.5
        cy = (max(ys) + min(ys)) * 0.5
        cz = (max(zs) + min(zs)) * 0.5
        # 半径（対角の半分）
        rx = (max(xs) - min(xs)) * 0.5
        ry = (max(ys) - min(ys)) * 0.5
        rz = (max(zs) - min(zs)) * 0.5
        r = max(rx, ry, rz, 1e-6)
        self._center = (cx, cy, cz)
        self._radius = float(r)

    def reset_view(self):
        self._q = (1.0, 0.0, 0.0, 0.0)
        self.fit_to_view()
        self._update()

    def _quat_mul(self, a, b):
        aw, ax, ay, az = a; bw, bx, by, bz = b
        return (
            aw*bw - ax*bx - ay*by - az*bz,
            aw*bx + ax*bw + ay*bz - az*by,
            aw*by - ax*bz + ay*bw + az*bx,
            aw*bz + ax*by - ay*bx + az*bw
        )

    def _axis_angle(self, axis, angle):
        # axis: (x,y,z) normalized, angle: radians
        x,y,z = axis
        s = math.sin(angle*0.5)
        return (math.cos(angle*0.5), x*s, y*s, z*s)

    def _quat_to_mat4(self, q):
        w,x,y,z = q
        xx, yy, zz = x*x, y*y, z*z
        xy, xz, yz = x*y, x*z, y*z
        wx, wy, wz = w*x, w*y, w*z
        # 列優先(OpenGL)の4x4
        return [
            1-2*(yy+zz),  2*(xy+wz),    2*(xz-wy),    0.0,
            2*(xy-wz),    1-2*(xx+zz),  2*(yz+wx),    0.0,
            2*(xz+wy),    2*(yz-wx),    1-2*(xx+yy),  0.0,
            0.0,          0.0,          0.0,          1.0,
        ]

    def _apply_trackball_drag(self, dx, dy):
        """画面ドラッグをカメラ座標系のヨー/ピッチ回転として self._q に右乗する"""
        if dx == 0 and dy == 0:
            return
        # 係数は好みで（今までと同じスピード感に近づける）
        k = float(getattr(self, "_trackball_speed", 0.015))
        # カメラローカル：横ドラッグ=Y軸（ヨー）、縦ドラッグ=X軸（ピッチ）
        ang_y = k * dx
        ang_x = k * dy

        # まずピッチ（X軸）、次にヨー（Y軸）をカメラ座標系で合成
        dq_pitch = self._axis_angle((1.0, 0.0, 0.0), ang_x)
        dq_yaw   = self._axis_angle((0.0, 1.0, 0.0), ang_y)
        # 右乗：現在姿勢の"後ろ"に積む → カメラローカル軸の回転になる
        dq = self._quat_mul(dq_yaw, dq_pitch)   # 順序：先にピッチ、次にヨー
        self._q = self._quat_mul(self._q, dq)

    def fit_to_view(self):
        """重心は動かさず、距離を決める。zoomは常に1.0＝fit状態を表す相対倍率にリセットする"""
        if not hasattr(self, "_radius"):
            self._recalc_bounds()
        r = max(1e-3, getattr(self, "_radius", 1.0))
        # 画角45°で入る十分距離：r/tan(fovy/2) の係数を余裕見て
        target_dist = (r / math.tan(math.radians(45.0) * 0.5)) * 1.8
        self._fit_dist = target_dist  # zoom=1.0の時のカメラ距離（perspective用）
        self.zoom = 1.0
        self.pan_x = self.pan_y = 0.0

    def toggle_wireframe(self):
        self.wireframe_mode = True
        self._update()

    def toggle_solid(self):
        self.wireframe_mode = False
        self._update()

    def toggle_projection(self):
        """Toggle between perspective and orthographic projection"""
        self.perspective_mode = not self.perspective_mode
        self._update()

    def initializeGL(self):
        gl.glEnable(gl.GL_DEPTH_TEST)
        gl.glDisable(gl.GL_CULL_FACE)
        gl.glDisable(gl.GL_LIGHTING)

    def resizeGL(self, w: int, h: int):
        # Viewport is finalized per-frame in paintGL (HiDPI aware).
        gl.glViewport(0, 0, max(1, w), max(1, h))

    def paintGL(self):
        if not OPENGL_AVAILABLE:
            return

        gl.glDisable(gl.GL_SCISSOR_TEST)
        gl.glEnable(gl.GL_DEPTH_TEST)

        fb_w, fb_h = self._framebuffer_size()
        gl.glViewport(0, 0, fb_w, fb_h)

        gl.glMatrixMode(gl.GL_PROJECTION); gl.glLoadIdentity()
        aspect = fb_w / float(fb_h) if fb_h else 1.0
        
        if self.perspective_mode:
            # Perspective projection
            glu.gluPerspective(45.0, aspect, 0.1, 10000.0)
        else:
            # Orthographic projection
            # zoom=1.0（fit状態）でperspectiveと同じ余裕（半径の1.8倍）が収まるようにする
            if hasattr(self, '_radius'):
                ortho_size = max(1e-3, self._radius) * 1.8 / max(1e-3, self.zoom)
            else:
                ortho_size = 5.0 / max(1e-3, self.zoom)
            
            if aspect >= 1.0:
                # Wide window
                gl.glOrtho(-ortho_size * aspect, ortho_size * aspect, 
                          -ortho_size, ortho_size, 0.1, 10000.0)
            else:
                # Tall window  
                gl.glOrtho(-ortho_size, ortho_size,
                          -ortho_size / aspect, ortho_size / aspect, 0.1, 10000.0)

        gl.glMatrixMode(gl.GL_MODELVIEW); gl.glLoadIdentity()
        gl.glClear(gl.GL_COLOR_BUFFER_BIT | gl.GL_DEPTH_BUFFER_BIT)
        glu.gluLookAt(0.0, 0.0, 0.0,  0.0, 0.0, -1.0,  0.0, 1.0, 0.0)

        # ===== 変換順序 =====
        # OpenGLの glTranslatef/glMultMatrixf は「後に呼んだ方が頂点に先に適用される」ため、
        # 頂点への適用順（1→4）にしたい変換は、コード上は逆順（4→1）で呼ぶ。
        # 1) モデルの重心を原点へ（回転の中心を図心に合わせる）
        # 2) 回転
        # 3) パン（画面平行移動）
        # 4) 視点からの距離（常に前方へ）。fit_to_view()が決めた距離をzoomで割った距離とする
        dist = getattr(self, "_fit_dist", 6.0) / max(1e-3, self.zoom)
        gl.glTranslatef(0.0, 0.0, -dist)                # 4を最初に呼ぶ→頂点には最後に適用
        gl.glTranslatef(self.pan_x, self.pan_y, 0.0)     # 3
        m = self._quat_to_mat4(self._q)
        gl.glMultMatrixf(m)                              # 2
        cx, cy, cz = getattr(self, "_center", (0.0, 0.0, 0.0))
        gl.glTranslatef(-cx, -cy, -cz)                   # 1を最後に呼ぶ→頂点には最初に適用（回転の中心=図心）

        # 表示モード
        if self.wireframe_mode:
            gl.glPolygonMode(gl.GL_FRONT_AND_BACK, gl.GL_LINE)
            gl.glDisable(gl.GL_CULL_FACE)
            gl.glDisable(gl.GL_LIGHTING)
            gl.glColor3f(0.0, 1.0, 0.0)
        else:
            gl.glPolygonMode(gl.GL_FRONT_AND_BACK, gl.GL_FILL)
            gl.glDisable(gl.GL_CULL_FACE)
            gl.glDisable(gl.GL_LIGHTING)
            gl.glColor3f(0.7, 0.8, 0.9)

        self._draw_mesh_immediate()

        # Draw coordinate axes in bottom-left with proper scene rotation
        self.draw_coordinate_axes()

    def _draw_mesh_immediate(self):
        if not self.vertices or not self.faces:
            return
        if self.wireframe_mode:
            for face in self.faces:
                gl.glBegin(gl.GL_LINE_LOOP)
                for vid in face:
                    x, y, z = self.vertices[int(vid)]
                    gl.glVertex3f(x, y, z)
                gl.glEnd()
        else:
            for face in self.faces:
                if len(face) < 3: continue
                v0 = self.vertices[int(face[0])]
                gl.glBegin(gl.GL_TRIANGLES)
                for i in range(1, len(face) - 1):
                    v1 = self.vertices[int(face[i])]
                    v2 = self.vertices[int(face[i + 1])]
                    nx, ny, nz = _face_normal(v0, v1, v2)
                    gl.glNormal3f(nx, ny, nz)
                    gl.glVertex3f(*v0); gl.glVertex3f(*v1); gl.glVertex3f(*v2)
                gl.glEnd()

    def draw_coordinate_axes(self):
        """ビューポートを一切変更しない座標軸オーバーレイ（NDC方式）"""
        # --- Save matrices & enables
        gl.glMatrixMode(gl.GL_PROJECTION); gl.glPushMatrix()
        gl.glMatrixMode(gl.GL_MODELVIEW);  gl.glPushMatrix()
        sc_on = gl.glGetBooleanv(gl.GL_SCISSOR_TEST)
        depth_on = gl.glGetBooleanv(gl.GL_DEPTH_TEST)
        light_on = gl.glGetBooleanv(gl.GL_LIGHTING)

        # === 1) NDCオーバーレイ投影（-1..+1） ===
        gl.glMatrixMode(gl.GL_PROJECTION)
        gl.glLoadIdentity()
        glu.gluOrtho2D(-1.0, 1.0, -1.0, 1.0)

        gl.glMatrixMode(gl.GL_MODELVIEW)
        gl.glLoadIdentity()

        # === 2) 画面左下へ配置（ピクセル→NDCに変換） ===
        vp = gl.glGetIntegerv(gl.GL_VIEWPORT)
        fb_w, fb_h = float(vp[2]), float(vp[3])
        dpr = float(getattr(self, "devicePixelRatioF", lambda: 1.0)())
        pad_px  = 10.0 * dpr
        size_px = 100.0 * dpr

        pad_ndc_x  = 2.0 * pad_px  / fb_w
        pad_ndc_y  = 2.0 * pad_px  / fb_h
        size_ndc_x = 2.0 * size_px / fb_w
        size_ndc_y = 2.0 * size_px / fb_h
        s = min(size_ndc_x, size_ndc_y) * 0.9

        # 左下基準：中心位置を計算
        cx = -1.0 + pad_ndc_x + s * 0.5
        cy = -1.0 + pad_ndc_y + s * 0.5

        # 配置（平行移動→回転→スケールの順）
        gl.glTranslatef(cx, cy, 0.0)
        # シーンの回転だけ反映（座標軸の向きを合わせる）- クォータニオン版
        m = self._quat_to_mat4(self._q)
        gl.glMultMatrixf(m)
        gl.glScalef(s, s, s)

        # === 3) 軸を描く ===
        if sc_on:   gl.glDisable(gl.GL_SCISSOR_TEST)
        if depth_on: gl.glDisable(gl.GL_DEPTH_TEST)
        if light_on: gl.glDisable(gl.GL_LIGHTING)

        gl.glLineWidth(2.0)
        gl.glBegin(gl.GL_LINES)
        # X (赤)
        gl.glColor3f(1.0, 0.0, 0.0); gl.glVertex3f(0,0,0); gl.glVertex3f(1,0,0)
        # Y (緑)
        gl.glColor3f(0.0, 1.0, 0.0); gl.glVertex3f(0,0,0); gl.glVertex3f(0,1,0)
        # Z (青)
        gl.glColor3f(0.0, 0.6, 1.0); gl.glVertex3f(0,0,0); gl.glVertex3f(0,0,1)
        gl.glEnd()

        # ===== ラベル描画（GLUT ビットマップ。なければスキップ） =====
        if _HAS_GLUT:
            gl.glDisable(gl.GL_DEPTH_TEST)  # 常に前面へ
            gl.glColor3f(1.0, 1.0, 1.0)

            # 軸長が 1.0 の想定。少し先に出す（"程よい"係数）
            label_offset = 1.08

            def _label(text, pos):
                gl.glRasterPos3f(*pos)
                for ch in text:
                    glut.glutBitmapCharacter(glut.GLUT_BITMAP_HELVETICA_18, ord(ch))

            _label("x", (label_offset, 0.0, 0.0))
            _label("y", (0.0, label_offset, 0.0))
            _label("z", (0.0, 0.0, label_offset))

            gl.glEnable(gl.GL_DEPTH_TEST)
        # GLUT が無い環境では何もしない（落とさない）

        # === 4) Restore states ===
        if light_on: gl.glEnable(gl.GL_LIGHTING)
        if depth_on: gl.glEnable(gl.GL_DEPTH_TEST)
        if sc_on:    gl.glEnable(gl.GL_SCISSOR_TEST)

        gl.glMatrixMode(gl.GL_MODELVIEW);  gl.glPopMatrix()
        gl.glMatrixMode(gl.GL_PROJECTION); gl.glPopMatrix()
        gl.glMatrixMode(gl.GL_MODELVIEW)

    def _framebuffer_size(self):
        if _LEGACY:
            return max(1, int(self.width())), max(1, int(self.height()))
        dpr = float(getattr(self, "devicePixelRatioF", lambda: 1.0)())
        return max(1, int(self.width() * dpr)), max(1, int(self.height() * dpr))

    def _update(self):
        if _LEGACY:
            self.updateGL()
        else:
            self.update()

    def mousePressEvent(self, e):
        self._last_pos = e.pos()
        self._last_button = e.button()

    def mouseMoveEvent(self, e):
        if self._last_pos is None:
            return
        dx = e.x() - self._last_pos.x()
        dy = e.y() - self._last_pos.y()
        if self._last_button == QtCore.Qt.LeftButton:
            # Rotate scene (affects both axes and center object)
            self._apply_trackball_drag(dx, dy)
        elif self._last_button == QtCore.Qt.RightButton:
            # Pan the main scene: 1画面px当たりの移動量を、現在表示中の半高さ
            # （fit_to_view基準の半径×1.8/zoom）とフレームバッファの半高さから求める。
            # 固定値だとメッシュの実寸によって感度が低すぎたり高すぎたりする。
            fb_w, fb_h = self._framebuffer_size()
            half_height = max(1e-3, getattr(self, "_radius", 1.0)) * 1.8 / max(1e-3, self.zoom)
            world_per_px = half_height / max(1.0, fb_h / 2.0)
            self.pan_x += dx * world_per_px
            self.pan_y -= dy * world_per_px
        self._last_pos = e.pos()
        self._update()

    def wheelEvent(self, e):
        delta = e.angleDelta().y() / 120.0
        # zoomはfit状態(=1.0)を基準にした相対倍率のため、範囲は絶対値のままで意味を持つ
        self.zoom *= math.pow(1.1, delta)
        self.zoom = max(0.02, min(self.zoom, 50.0))
        self._update()

    def keyPressEvent(self, e):
        k = e.key()
        if k == QtCore.Qt.Key_W:
            self.wireframe_mode = True
            print("Key_W")
            self._update()
        elif k == QtCore.Qt.Key_S:
            self.wireframe_mode = False
            print("Key_S")
            self._update()
        elif k == QtCore.Qt.Key_F:
            # Fit to view
            if hasattr(self, "fit_to_view"):
                self.fit_to_view()
            self._update()
        elif k == QtCore.Qt.Key_R:
            # Reset all: scene quaternion, pan, zoom (fit to pane)
            self._q = (1.0, 0.0, 0.0, 0.0)
            self.fit_to_view()
            self._update()
        elif k == QtCore.Qt.Key_P:
            # Toggle projection mode
            self.toggle_projection()
        else:
            super().keyPressEvent(e)

    # Compatibility aliases
    def __getattr__(self, name):
        if name.endswith('_') and hasattr(self, name[:-1]):
            return getattr(self, name[:-1])
        raise AttributeError(f"'{type(self).__name__}' object has no attribute '{name}'")

    def __setattr__(self, name, value):
        if name.endswith('_') and hasattr(self, name[:-1]):
            setattr(self, name[:-1], value)
        else:
            super().__setattr__(name, value)


def _face_normal(v0: Vec3, v1: Vec3, v2: Vec3):
    x0,y0,z0 = v0; x1,y1,z1 = v1; x2,y2,z2 = v2
    ux, uy, uz = (x1-x0, y1-y0, z1-z0)
    vx, vy, vz = (x2-x0, y2-y0, z2-z0)
    nx, ny, nz = (uy*vz - uz*vy, uz*vx - ux*vz, ux*vy - uy*vx)
    norm = math.sqrt(nx*nx + ny*ny + nz*nz)
    return (nx/norm, ny/norm, nz/norm) if norm > 1e-12 else (0,0,1)


# --------------- standalone test ---------------
if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    w = MeshViewer3D()
    w.setWindowTitle("GM3D Independent Viewer - Working Version")
    # Start with empty viewer - shows rotating axes in bottom-left and interactive cube in center
    w.show()
    print("Independent viewer started. Bottom-left axes rotate with scene, center object has independent controls.")
    print("Keys: W=wireframe, S=solid, F=fit view, R=reset object, P=toggle projection")
    sys.exit(app.exec_())