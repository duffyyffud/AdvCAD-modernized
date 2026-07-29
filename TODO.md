# TODO

このワークスペース（AdvCAD-modernized）の、実際に確認した事実に基づく残作業一覧。
推測で書いた項目は無い。断定できない所は「未確認」と明記する。

## 解決済み（前回2026-07-21作成時点では未対応だった項目）

### 1. cmake による生成の失敗 → 修正済み・commit `be5d221`（2026-07-25）
- `command/CMakeLists.txt:53-55` の参照先を `scripts/optimize_mesh_size.py` に直した（2026-07-25付けで再度`apps/optimize_mesh_size.py`へ変更、下記項目8参照）。
- 根拠: 直した状態で `build/` を作り直し `cmake .. && make -j4` が成功する事、
  `./command/advcad sample/block.gm3d` が `Success: 46 triangles` で終わる事を確認済み（2026-07-21）。

### 2. `CLAUDE.md` の古い記載 → 改定済み
- 成功率・作業ディレクトリ・道具のパス等、確認できた古い記載は2026-07-21付けで直した。

## 確認済み・対応が要る

### 12. shaft 2モデルの面が閉じていない（`shaft/air_up2_mid_out_01.gm3d`・`shaft/cyclic_mag_body_01.gm3d`）
- 項目11の穴あき面欠陥修正（commit `d753713`）の対象外。`dev/mesh_checker.py`実測（2026-07-29）で両モデルとも境界辺が残る（`air_up2_mid_out_01.gm3d`は境界辺9・`cyclic_mag_body_01.gm3d`は境界辺16）ことを確認済み。連結成分数はどちらも1（分裂はしていない）。
- 未着手。原因調査はこれから。

### 13. 実験枝 `experiment/standard-cdt-always` の後始末
- 項目11の修正（selectStrategy()のSTANDARD_CDT固定・catch節でのperformFallbackTriangulation()呼び出し・fitBoundary()救済パス削除・出力段WH_NO_INDEX除外の4点）は、この枝の内容を検証した上でmasterへ個別に統合した（commit `d753713`、branchのmergeではない）。
- この枝には統合元のロジックに加え、計測専用の計装（`_measureRescueCount`・面単位の`MEASURE triangle centroid=...`ログ等）が残っており、masterには意図的に持ち込んでいない。
- 枝はローカルにのみ存在（`git branch -r`でorigin側に同名branchは無いことを確認済み、2026-07-29）。どう扱うか（削除・保管のみ・別途参照用に残す等）は未確定。Akioの判断待ち。

## 解決済み（2026-07-25対応、commit `eeff5ce`）

### 4. `run_tests.sh`（ワークスペース直下）が壊れている → 解消済み
- `dev/test_regression.py` を呼ぶよう修正した。

### 5. `dev/test_regression.py` はそのままでは動かない → 解消済み
- `project_root` をスクリプト自身の位置から解決するよう修正した（`Path(__file__).resolve().parent.parent`）。

### 6. 同名の回帰確認スクリプトが2か所にあり、内容が食い違う → 解消済み
- `tests/regression/`（`command/advcad`・`tests/data/shaft`という、CMake移行前かつgit追跡された古い実行ファイルを参照する内容だった）を削除し、`dev/test_regression.py`を正本とした。
- 根拠: `python3 dev/test_regression.py`・`bash run_tests.sh` とも19/19成功を確認済み（2026-07-25）。

## 解決済み（2026-07-25対応）

### 8. `apps/optimize_mesh_size.py` と `scripts/optimize_mesh_size.py` が完全に同一の重複ファイル → 解消済み
- Akio指示（SSOT徹底、symbolic link禁止）により`scripts/optimize_mesh_size.py`を削除し、`apps/optimize_mesh_size.py`を唯一の実体とした。
- `command/CMakeLists.txt`の`configure_file`参照先を`apps/optimize_mesh_size.py`に修正し、`cmake .. && make -j4`の成功、`build/command/optimize_mesh_size.py`の生成、`python3 apps/optimize_mesh_size.py`単体実行の成功を確認済み（2026-07-25）。

### 9. `*.md`整理統合（fork agent調査、2026-07-25） → 対応済み
- `MODERNIZATION.md`は`CHANGELOG.md`と8割重複しWH_ASSERT方針が矛盾していた為、`CHANGELOG.md`へ統合し削除。
- `docs/CODEBASE_KNOWLEDGE.md`はEdit道具の教訓が`CLAUDE.md`・`GOOD_PRACTICE.md`と3重複していた為削除。`GOOD_PRACTICE.md`の該当節は`CLAUDE.md`・`docs/DEBUG_META_KNOWLEDGE.md`への参照に置き換えた。
- `docs/CMAKE_MIGRATION_PCH_ROBUSTNESS_PLAN.md`は前提(`src/`集約)が実際の経緯(commit `95f12b4`)と完全に逆転しており生きた内容が無い為削除(旧項目3)。
- `docs/THEORETICAL_MANUAL.md`の閾値`> 6`(現`>= 6`)・成功率84.2%表記を是正。
- `docs/README.md`・`scripts/README.md`の陳腐化(存在しないサブディレクトリ、削除済み`optimize_mesh_size.py`の記載)を是正。
- 統合しなかった物: `PPA_SETUP.md`(97.5%表記等は別途修正要)・`debian_official.md`・`INSTALL.md`(読者が異なる為現状維持)・`web-app/DESIGN_PLAN_MCP.md`(現状投資しない方針)。`scripts/visualize_face7.py`と`dev/visualize_face7.py`の重複は今回のmd整理の範囲外、別件。

## 解決済み（2026-07-22〜23の陳腐化点検で対応済み）

- `docs/DEGENERATE_TRIANGLE_FIX_PLAN.md`・`docs/MESH_FAILURE_ANALYSIS_PLAN.md`・`docs/ZERO_VECTOR_FIX_PLAN.md`: 現状との整合を点検済み。数値・パスの陳腐化箇所に注記を追加し、全て`docs/`へ移動した。

## 解決済み（2026-07-26対応、非`.md`ファイルの整理整頓）

### 10. root直下の目的不明ファイル・ディレクトリ（fork agent調査） → 対応済み
- `my_secret_key.asc`(0バイト、常に空だった事を`git log`で確認)・`reorganize.sh`(採用されなかった`src/`集約案)を削除。
- `__pycache__`配下の`.pyc`2件(`.gitignore`をすり抜けて追跡されていた)を追跡除去。
- `face5_debug_tests/`・`generated_tests/`・`stress_tests/`(`dev/`の生成スクリプトが実際に使う現役データ)を`dev/`配下へ移動し、参照元4スクリプトのパスを修正。
- `reduced_tests/`: 中身を読み、`cyclic_mag_body_01.gm3d`のFace 5混合三角形問題(Breakthrough #5・commit `7c1e226`)を手作業で切り詰めた最小再現ケース(6頂点sheet・小規模座標が症状条件と一致)と特定。生成スクリプトは無いが回帰確認材料として価値がある為、`dev/reduced_tests/`へ移動し`dev/README.md`に記載した。
- Windows関連(`advcad-windows-v0.13.0/`・`.zip`・`toolchain-mingw64.cmake`)・`test_opengl*.py`・`Pipfile`/`Pipfile.lock`は、当初「参照が無い＝不要」と誤判断したが、Akio確認の結果いずれも現役(Windowsが主要実行環境、WSL2のOpenGL制約検証、pipenvによるPython環境構築)と判明。削除せず、`CLAUDE.md`・`dev/README.md`・`apps/README.md`にそれぞれ文書化し、`test_opengl*.py`は`dev/`へ移動した。
- `benchmark.cpp`: 必要性を精査し、現行`WH/`に対して再ビルド・実行して動作する事を確認(CMakeLists.txtには未組み込みのまま)。`MODERNIZATION.md`統合時に脱落していた再ビルド手順を`CHANGELOG.md`へ復元した。
- PPA関連ファイルを`ppa/`へ集約。単純移動ではなく、`create_ppa.sh`が誤ってroot直下の`debian/`(`build_deb.sh`用のDEBIAN/usr構成、PPA用のdebhelper構成とは別物)を参照していた事、`tar --exclude`のオプション順序が誤っていた事を発見し、実際に成功した過去のアップロード(`advcad_0.13.0-1ubuntu1.debian.tar.xz`)から本物のdebhelper`debian/`を復元して`ppa/debian/`とした上で修正。`advcad_gpg_public.asc`は失効済み旧鍵の公開鍵だった為、新鍵(`FA623FE1...`)の書き出しに差し替えた。
- 漏洩した秘密鍵(`advcad_gpg_secret.asc`)本体を、`git filter-repo`で全branch・全tagの履歴から除去し、force pushで反映済み(2026-07-27)。ローカルの不要な実体・`my_secret_key.asc`(常に空)も削除。
- root直下の残り(`build_deb.sh`・`debian/`・`advcad_0.13.0_amd64.deb`・`copyright`・`debian_official.md`・`apt-repo/`・`setup_apt_repo.sh`・`test_package.sh`を`deb/`、Windows関連を`windows/`、`benchmark.cpp`を`dev/`)を集約し、root直下の項目数を43件→29件にした(2026-07-27)。
- `sample/shaft/coil_01_1.gm3d`が回帰確認の対象から漏れていた件(旧項目7) → 解消済み。実行して失敗しない事を確認(メッシュサイズ1.0で4496三角形、`exit=0`)した上で`dev/test_regression.py`の対象一覧に追加。19/19→20/20成功。`CLAUDE.md`の該当箇所も20/20に更新した(2026-07-27)。

## 解決済み（2026-07-29対応、commit `d753713`、tag `v0.16.0-hole-mesh-fix`）

### 11. 穴あき面欠陥（robust CDTの未割当三角形が救済されて出力に混入する）→ 解消済み
- 経緯: `sample/debug_test07_29.gm3d`（外箱から貫通穴をsubtractした形状）で、`optimize_mesh_size.py`がメッシュサイズを変えても結果が変化しない不具合の調査から発端。原因を辿ったところ、`robust_cdt.cc`の`fitBoundary()`がドメイン未割当（`WH_NO_INDEX`）の三角形へ一律`domainId=1`を割り当てて出力段のフィルタ(`domainId()==0`のみ除外)をすり抜けさせていたことが判明。
- 修正4点（1 commit）: (1) `selectStrategy()`をSTANDARD_CDT固定にし、根拠のない閾値50/100・`estimateGeometryComplexity()`を撤去 (2) STANDARD_CDT失敗時のcatch節で`performFallbackTriangulation()`を実際に呼ぶよう修正（従来は警告を出すだけで呼んでいなかった） (3) `fitBoundary()`の`domainId=1`救済パスを削除 (4) 出力段フィルタで`WH_NO_INDEX`三角形を明示的に除外し、除外数を標準エラーへ報告するよう変更。
- 検証: トンネル箱（境界辺0・連結成分1・穴内三角形混入0・WH_NO_INDEX除外0枚）、20モデル回帰20/20、AddressSanitizer+UndefinedBehaviorSanitizerビルドでも20/20・検出0件。実験枝`experiment/standard-cdt-always`（同じ4変更に計測専用の計装を加えた版）の実測値と完全一致することを確認済み。
- 未解決のまま残った物は項目12・13を参照。

## 自動デバッグエージェント計画の次アクション

`CHATg_214_01.md` [5] 系列の現在地（2026-07-27）。転記でなく参照とする。`CHATg_214_01.md`本体は変更しない。

1. [5-1]〜[5-3]・[4-1]〜[4-5]の確認: 完了。根拠は2026-07-27の advcad codebase構造把握セッション（本チャット記録）を参照。
2. 次のアクション: [5-8]（POC仕様のgo/no-go再評価）。
3. [5-4]（PatchSize伝播の深掘り）・[5-5]（データ構造・所有権の確認）: 廃止ではなく延期。障害の症状に応じて調査範囲を拡張する際に実施する。優先調査箇所の判断基準は `docs/AutoDebugAgentsImplementPlan.md` サブクエリ14（493行目〜）を参照。

## 未確認(要調査、断定しない)

（無し）

---
最終更新: 2026-07-29（Claude Code）。本文中に挙げた根拠（コミット・ファイル・行番号・実行結果）に基づく。
