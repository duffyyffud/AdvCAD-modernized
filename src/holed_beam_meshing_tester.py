import os
import pandas as pd
import numpy as np
import logging
from Elastic_plastic_controller import ElPlTesterBeamWHole



def create_beam_10x50x200_2(path: str, thickness: float, length: float,
                          width: float, current_center_z: float,
                          radius: float):
    # 円孔付き梁
    # パラメータで円孔の位置がz方向に変化する
    # 一旦直方体を作り、その後円柱を引き算する(第2の方法)。
    script = f'box 0. 0. 0.  {thickness} {width} {length}\n'
    script += f'circle  -1. 0 {current_center_z}  0 0 {radius}  1 0 0 36\n'
    script += f'extrude {thickness + 1.} 0 0\n'
    script += 'subtract\n'
    print(f'gm3d file path = {path}.')
    with open(path, mode='w') as gm3d_f:
        gm3d_f.write(script)
    pass
# end create_beam_10x50x200_2




def main():
    np.set_printoptions(formatter={'float': '{:0.4e}'.format, 'int': '{:05d}'.format})
    version = '1.21'
    distance = 4.0
    yield_stress = 225
    tensile_strength = 265
    poisson_ratio = 0.3
    youngs_modulus = 205800
    hardening_coeff = 20580
    # AdvOnWinのインストールフォルダ
    adv_base_dir = 'C:/tools/AdvOnWin0_43b'
    # Change next line:結果ファイル群を置くフォルダ
    out_dir_base = '../examples/beam_10x50x200_2'
    my_angle = 90
    initial_n_steps = 20
    # ADV_CAD　形状定義ファイル
    gm3d_name = 'beam_10x50x200_2.gm3d'
    thickness = 10.
    width = 50.
    length = 200.
    # 1ステップで0.5mmずつ移動させる (150-100)/100 = 0.5
    center_z_min = 100.
    center_z_max = 150.
    radius = 15.
    n_parametric_cases = 1
    my_case_id = 101
    # 境界条件は、対話式に与えるので、ここでは記述出来ない。

    ept = ElPlTesterBeamWHole(version, yield_stress, poisson_ratio,
                              distance, youngs_modulus, hardening_coeff,
                              adv_base_dir, out_dir_base, my_angle, initial_n_steps,
                              gm3d_name, thickness, width, length,
                              center_z_min, center_z_max, radius,
                              n_parametric_cases, tensile_strength, my_case_id)
    subcase_id = 0
    for i_th in range(subcase_id):
        ept.adv_on_win.append(None)
    pass
    ept.init_adv_on_win(subcase_id=subcase_id)
    ept.make_surface_patch_3(150 - 0.5 * (72 + subcase_id))
# end main


if __name__ == '__main__':
    main()
pass
