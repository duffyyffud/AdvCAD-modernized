#!/usr/bin/env python3
"""出力メッシュ検査器（pch形式）。

docs/AutoDebugAgentsImplementPlan.md Step 13 の検査項目のうち、
頂点インデックス範囲・零面積三角形・重複三角形・境界辺・非多様体辺・
連結成分数の6項目を実装する。

使い方:
  python3 dev/mesh_checker.py <pchファイル>   単一のpchファイルを検査する
  python3 dev/mesh_checker.py                 回帰20モデルの出力を生成して一括検査する
"""

import subprocess
import sys
import tempfile
from pathlib import Path

ZERO_AREA_TOLERANCE = 1e-12

# 回帰20モデルとdev/test_regression.pyで確認済みのメッシュサイズ
REGRESSION_MODELS = [
    ("block.gm3d", 0.1414213562373095),
    ("test_1.gm3d", 1.0),
    ("test_2.gm3d", 1.0),
    ("test_3.gm3d", 1.0),
    ("test_4.gm3d", 1.0),
    ("test_5.gm3d", 1.5),
    ("test_6.gm3d", 1.0),
    ("test_7.gm3d", 1.0),
    ("cake/Magnetic0.gm3d", 1.0),
    ("cake/Magnetic1.gm3d", 1.0),
    ("cake/Magnetic2.gm3d", 1.0),
    ("cake/Magnetic3.gm3d", 1.0),
    ("shaft/air_practice.gm3d", 1.0),
    ("shaft/air_up2_mid_out_01.gm3d", 1.0),
    ("shaft/air_up2_top_01.gm3d", 1.0),
    ("shaft/coil_01.gm3d", 1.0),
    ("shaft/coil_01_1.gm3d", 1.0),
    ("shaft/coil_01_mm.gm3d", 1.0),
    ("shaft/coil_02.gm3d", 1.0),
    ("shaft/cyclic_mag_body_01.gm3d", 1.0),
]


def read_pch(pch_path):
    """pchファイルを読み、(頂点座標のリスト, 三角形の頂点インデックスのリスト)を返す。

    形式: nNodes / x y z を nNodes行 / nTriangles / 頂点index3個 を nTriangles行。
    先頭行が pcm 形式(nNodes 0 1)であっても、最初のトークンだけを nNodes として読む。
    """
    with open(pch_path) as f:
        tokens = f.read().split()

    position = 0
    node_count = int(tokens[position])
    position += 1

    nodes = []
    for _ in range(node_count):
        x = float(tokens[position])
        y = float(tokens[position + 1])
        z = float(tokens[position + 2])
        nodes.append((x, y, z))
        position += 3

    triangle_count = int(tokens[position])
    position += 1

    triangles = []
    for _ in range(triangle_count):
        i0 = int(tokens[position])
        i1 = int(tokens[position + 1])
        i2 = int(tokens[position + 2])
        triangles.append((i0, i1, i2))
        position += 3

    return nodes, triangles


def triangle_area(nodes, triangle):
    """三角形の面積を返す。頂点indexが範囲外ならNoneを返す。"""
    node_count = len(nodes)
    if any(index < 0 or index >= node_count for index in triangle):
        return None

    p0 = nodes[triangle[0]]
    p1 = nodes[triangle[1]]
    p2 = nodes[triangle[2]]

    ux, uy, uz = p1[0] - p0[0], p1[1] - p0[1], p1[2] - p0[2]
    vx, vy, vz = p2[0] - p0[0], p2[1] - p0[1], p2[2] - p0[2]

    cx = uy * vz - uz * vy
    cy = uz * vx - ux * vz
    cz = ux * vy - uy * vx

    return 0.5 * (cx * cx + cy * cy + cz * cz) ** 0.5


def check_unreferenced_vertices(nodes, triangles):
    """どの三角形からも参照されていない頂点indexを集める。"""
    referenced = set()
    for triangle in triangles:
        referenced.update(triangle)
    return [i for i in range(len(nodes)) if i not in referenced]


def check_vertex_index_range(nodes, triangles):
    """頂点indexが[0, node_count-1]を外れる(三角形index, 頂点index)を集める。"""
    node_count = len(nodes)
    out_of_range = []
    for triangle_index, triangle in enumerate(triangles):
        for vertex_index in triangle:
            if vertex_index < 0 or vertex_index >= node_count:
                out_of_range.append((triangle_index, vertex_index))
    return out_of_range


def check_zero_area_triangles(nodes, triangles):
    """零面積三角形(面積がZERO_AREA_TOLERANCE以下)の三角形indexを集める。"""
    zero_area = []
    for triangle_index, triangle in enumerate(triangles):
        area = triangle_area(nodes, triangle)
        if area is not None and area <= ZERO_AREA_TOLERANCE:
            zero_area.append(triangle_index)
    return zero_area


def check_duplicate_triangles(triangles):
    """同じ3頂点の組み合わせ(向き無視)が複数回現れる三角形を集める。"""
    seen_count = {}
    for triangle in triangles:
        key = tuple(sorted(triangle))
        seen_count[key] = seen_count.get(key, 0) + 1
    return {key: count for key, count in seen_count.items() if count > 1}


def build_edge_triangle_map(triangles):
    """辺(頂点2個の組、向き無視)ごとに、それを持つ三角形indexの一覧を作る。"""
    edge_triangle_map = {}
    for triangle_index, triangle in enumerate(triangles):
        vertex_pairs = [
            (triangle[0], triangle[1]),
            (triangle[1], triangle[2]),
            (triangle[2], triangle[0]),
        ]
        for a, b in vertex_pairs:
            edge_key = tuple(sorted((a, b)))
            edge_triangle_map.setdefault(edge_key, []).append(triangle_index)
    return edge_triangle_map


def check_boundary_edges(edge_triangle_map):
    """三角形をちょうど1個だけ持つ辺(境界辺)を集める。"""
    return [edge for edge, tris in edge_triangle_map.items() if len(tris) == 1]


def check_non_manifold_edges(edge_triangle_map):
    """三角形を3個以上持つ辺(非多様体辺)を集める。"""
    return [edge for edge, tris in edge_triangle_map.items() if len(tris) >= 3]


def count_connected_components(triangle_count, edge_triangle_map):
    """辺を共有する三角形どうしを連結として、連結成分数を数える(union-find)。"""
    parent = list(range(triangle_count))

    def find(i):
        while parent[i] != i:
            parent[i] = parent[parent[i]]
            i = parent[i]
        return i

    def union(i, j):
        root_i, root_j = find(i), find(j)
        if root_i != root_j:
            parent[root_i] = root_j

    for tris in edge_triangle_map.values():
        for i in range(1, len(tris)):
            union(tris[0], tris[i])

    return len(set(find(i) for i in range(triangle_count)))


def check_mesh(pch_path):
    """pchファイル1件を検査し、結果を辞書で返す。"""
    nodes, triangles = read_pch(pch_path)

    out_of_range = check_vertex_index_range(nodes, triangles)
    unreferenced = check_unreferenced_vertices(nodes, triangles)
    zero_area = check_zero_area_triangles(nodes, triangles)
    duplicates = check_duplicate_triangles(triangles)
    edge_triangle_map = build_edge_triangle_map(triangles)
    boundary_edges = check_boundary_edges(edge_triangle_map)
    non_manifold_edges = check_non_manifold_edges(edge_triangle_map)
    component_count = count_connected_components(len(triangles), edge_triangle_map)

    return {
        "node_count": len(nodes),
        "triangle_count": len(triangles),
        "out_of_range_count": len(out_of_range),
        "unreferenced_vertex_count": len(unreferenced),
        "zero_area_count": len(zero_area),
        "duplicate_triangle_count": len(duplicates),
        "boundary_edge_count": len(boundary_edges),
        "non_manifold_edge_count": len(non_manifold_edges),
        "connected_component_count": component_count,
    }


def print_single_result(model_name, result):
    print("モデル: " + model_name)
    print("頂点数: %d" % result["node_count"])
    print("三角形数: %d" % result["triangle_count"])
    print("範囲外index: %d" % result["out_of_range_count"])
    print("未参照頂点: %d" % result["unreferenced_vertex_count"])
    print("零面積三角形: %d" % result["zero_area_count"])
    print("重複三角形: %d" % result["duplicate_triangle_count"])
    print("境界辺: %d" % result["boundary_edge_count"])
    print("非多様体辺: %d" % result["non_manifold_edge_count"])
    print("連結成分数: %d" % result["connected_component_count"])


def run_regression_batch():
    """回帰20モデルについてadvcadを実行し、生成したpchを検査して結果表を表示する。"""
    project_root = Path(__file__).resolve().parent.parent
    advcad_exe = project_root / "build" / "command" / "advcad"

    if not advcad_exe.exists():
        raise FileNotFoundError("advcad実行ファイルが見つからない: " + str(advcad_exe))

    results = []
    with tempfile.TemporaryDirectory() as output_dir:
        output_dir_path = Path(output_dir)
        for model_file, mesh_size in REGRESSION_MODELS:
            geometry_path = project_root / "sample" / model_file
            output_path = output_dir_path / (model_file.replace("/", "_").replace(".gm3d", ".pch"))

            run_result = subprocess.run(
                [str(advcad_exe), str(geometry_path), str(output_path), str(mesh_size)],
                capture_output=True, text=True,
            )

            if run_result.returncode != 0 or not output_path.exists():
                results.append({"model": model_file, "error": "advcad実行に失敗した(終了コード %d)" % run_result.returncode})
                continue

            result = check_mesh(output_path)
            result["model"] = model_file
            result["error"] = None
            results.append(result)

    print_results_table(results)


def print_results_table(results):
    columns = ["モデル", "頂点数", "三角形数", "範囲外index", "未参照頂点", "零面積", "重複三角形", "境界辺", "非多様体辺", "連結成分数"]
    print("\t".join(columns))
    for r in results:
        if r.get("error"):
            print(r["model"] + "\t" + r["error"])
            continue
        print("\t".join(str(x) for x in [
            r["model"], r["node_count"], r["triangle_count"],
            r["out_of_range_count"], r["unreferenced_vertex_count"], r["zero_area_count"],
            r["duplicate_triangle_count"], r["boundary_edge_count"],
            r["non_manifold_edge_count"], r["connected_component_count"],
        ]))


def main():
    if len(sys.argv) == 2:
        pch_path = Path(sys.argv[1])
        result = check_mesh(pch_path)
        print_single_result(str(pch_path), result)
    elif len(sys.argv) == 1:
        run_regression_batch()
    else:
        print("使い方: mesh_checker.py [pchファイル]  (引数なしなら回帰20モデルを生成して検査する)")
        sys.exit(1)


if __name__ == "__main__":
    main()
