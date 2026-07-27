# PROJECT_02 (2026-07-27): TODO.md記載の残課題(2-3〜2-7・7)全件解消、`*.md`/非`*.md`のroot整理、advcadデバグのメタ知識抽出、漏洩GPG秘密鍵の全履歴除去とforce push

## 1. やった事

前セッション(`PROJECT_01.md`)は、`CHAT_OPERATIONS.md`新設・`CLAUDE.md`改定・秘密鍵漏洩の発見とローテーション着手まで進み、`advcad_gpg_secret.asc`の`git rm --cached`が単独commitとして成立したか未確認のまま、複数の未commit変更(文書修正一式)を残して終わっていた。本セッションはその残課題を1件ずつ解消し、途中でAkio指摘によりデバグメタ知識の抽出・非`.md`ファイルの整理・漏洩鍵の全履歴除去(force push)まで対象を広げ、最終的に回帰確認20/20成功、root直下43件→29件、漏洩鍵のGitHub上での到達可能性ゼロ、という状態に到達した。

- (1) `PROJECT_01.md`を読み、2-1(鍵除去commit)は既に`git log`上で単独成立(commit、現hash`e8aa956`)しておりunpushのみが残課題である事を確認。2-2(鍵ローテーション)もAkio自身の端末で新鍵`FA623FE12512A6B03C125A3B28904C6D0CDEA097`生成・旧鍵`7EDE4E783F7F55F6B5201AF5D96BDCC87890DE84`のローカル失効・`revoke-7EDE4E78.asc`作成まで既に完了済みと`gpg --list-secret-keys`で確認した。
- (2) 2-3(`command/CMakeLists.txt`の`configure_file`が旧パスを参照)をcommit(現hash`98a55c8`)。
- (3) 2-4〜2-6(`run_tests.sh`の破損・`dev/test_regression.py`の旧パス直書き・`tests/regression/`との内容相違)をまとめて解消。`dev/test_regression.py`の`project_root`を`Path(__file__).resolve().parent.parent`に変更し、`run_tests.sh`が`dev/test_regression.py`を呼ぶよう修正、`tests/regression/`(CMake移行前のgit追跡済み`command/advcad`バイナリを参照する内容だった)を削除。`python3 dev/test_regression.py`・`bash run_tests.sh`とも19/19成功を確認しcommit(現hash`840afa8`)。
- (4) 前セッションで内容確定済みだった文書修正一式(`CHANGELOG.md`・`CLAUDE.md`・`GOOD_PRACTICE.md`・`INSTALL.md`・`README_en.md`・`apps/README.md`・`docs/*_PLAN.md`3件)をcommit(現hash`f239d0a`)。
- (5) 2-7(`apps/optimize_mesh_size.py`と`scripts/optimize_mesh_size.py`の完全一致重複)を解消。Akio指示(SSOT徹底・symbolic link禁止)により`scripts/`側を削除し`command/CMakeLists.txt`の参照先を`apps/`に変更、`cmake .. && make -j4`成功・`build/command/optimize_mesh_size.py`生成・単体実行成功を確認しcommit(現hash`726410e`)。
- (6) Akio依頼により`~/workspace/wsPy/AutoDevByMultiAgents/`をfork agentで調査。C++デバグ支援に直接使えるコードは無かったが、`prompts/CLAUDE_CODE_IdealPhasedTaskTemplate.md`の段階的デバグ手順(Phase 0事実確認〜Phase 5報告)と、AdvCAD側に無い「累積3回の差し戻しで打ち切りエスカレート(棚上げ)」の基準を発見。
- (7) Akio指摘(「解決済みの不具合からデバグ方法のメタ知識を抽出したい、事実の陳腐化是正とは別」)を受けて方針を修正し、commit `7c1e226`(Face 5、閾値`> 6`→`>= 6`)・`7c1d651`(domain ID初期化漏れ)・`f0d9ab5`(小規模座標判定)・`efca872`(入力データの妥当性)・`b64606a`(2D/3D非対称な防御漏れ)の実コミットメッセージを読み込み、7点の一般則を抽出。(6)の段階的手順・打ち切り基準と合わせて`docs/DEBUG_META_KNOWLEDGE.md`を新規作成しcommit(現hash`e60c901`)。
- (8) fork agentにより workspace内`*.md`全24件を調査(重複・矛盾・統合可否)。`MODERNIZATION.md`(`CHANGELOG.md`と8割重複、WH_ASSERT方針が矛盾)・`docs/CODEBASE_KNOWLEDGE.md`(Edit道具の教訓が`CLAUDE.md`・`GOOD_PRACTICE.md`と3重複)・`docs/CMAKE_MIGRATION_PCH_ROBUSTNESS_PLAN.md`(前提がcommit `95f12b4`で完全に逆転済み)を削除し、`docs/THEORETICAL_MANUAL.md`(閾値`> 6`・成功率84.2%表記)・`docs/README.md`(存在しないサブディレクトリ記載)・`scripts/README.md`(削除済みファイルの記載)を是正。`GOOD_PRACTICE.md`の重複節は`CLAUDE.md`・`docs/DEBUG_META_KNOWLEDGE.md`への参照に置き換え。commit(現hash`f619ca5`)。
- (9) `PPA_SETUP.md`の目的分析、およびPPAという用語自体の説明をAkioに提供(コード変更無し)。
- (10) fork agentによりroot直下の非`.md`ファイル・ディレクトリを調査。`my_secret_key.asc`(常に0バイト)・`reorganize.sh`(採用されなかった`src/`集約案)を削除、`__pycache__`内`.pyc`2件の追跡除去、`face5_debug_tests/`・`generated_tests/`・`stress_tests/`を`dev/`へ移動し参照元4スクリプトのパスを修正。Windows関連(`advcad-windows-v0.13.0/`・`toolchain-mingw64.cmake`)・`test_opengl*.py`・`Pipfile`/`Pipfile.lock`は当初「参照無し=不要」と誤判断したが、Akioから(1)Windowsが主要実行環境である事(2)WSL2のOpenGL表示制約(3)pipenvによるPython環境構築、の3点を訂正され、`CLAUDE.md`・`dev/README.md`・`apps/README.md`に文書化。`benchmark.cpp`は再ビルド・実行して動作する事を確認し必要と判断。PPA関連ファイルを`ppa/`へ集約する過程で、`create_ppa.sh`が`build_deb.sh`用の`DEBIAN/usr`構成`debian/`を誤って参照していた事(debhelper用構成ではない)と、`tar --exclude`のオプション順序誤りを発見。実際に成功した過去アップロード`advcad_0.13.0-1ubuntu1.debian.tar.xz`から本物のdebhelper`debian/`(`control`・`rules`・`compat`・`changelog`)を復元し`ppa/debian/`とした。`advcad_gpg_public.asc`は失効済み旧鍵の公開鍵だった為、新鍵の書き出しに差し替え。commit(現hash`c5bdeec`)は`git add -A`が存在しないパス指定で失敗し内容変更が全く含まれておらず、`git status`の再確認で発覚、commit(現hash`97f63da`)で修正。
- (11) `reduced_tests/`の中身を読み、`cyclic_mag_body_01.gm3d`のFace 5混合三角形問題(commit `7c1e226`)を手作業で切り詰めた最小再現ケース(6頂点sheet・小規模座標)と特定。`dev/reduced_tests/`へ移動し`dev/README.md`に記載((10)のcommitに含む)。
- (12) Akio指摘「advcad_gpg_secret.ascは公開されているのでは」を受けて調査。commit(現`5902b2d`相当)で追加された同ファイルが`origin/master`から到達可能である事を`git merge-base --is-ancestor`で確認し、公開されたままである事を確定。
- (13) `git filter-repo`(pip3でインストール)により`advcad_gpg_secret.asc`を全branch・全tagのローカル履歴から除去。ローカルの実体ファイルも削除。GitHub側fine-grained PATに"Contents"権限(Read and write)が無く`git push --force`が403で失敗した為、Akioにトークン設定変更手順を提示し反映後、`master`・他4 branch(`claude/verify-repo-behavior-YcNzN`・`v0.14.2-pipeline-fix`・`v0.15-3d-viewer-dev`・`v0.15.0-gui-enhancements`)へforce push。この時点でtag(27件)のforce pushは未完了のまま一旦停止。
- (14) fork調査済みの内容のみで、root直下の残り(`build_deb.sh`・`debian/`・`advcad_0.13.0_amd64.deb`・`copyright`・`debian_official.md`・`apt-repo/`・`setup_apt_repo.sh`・`test_package.sh`を`deb/`へ、Windows関連を`windows/`へ、`benchmark.cpp`を`dev/`へ)を集約し、`__pycache__`削除・`face7_*.debug`(advcad実行毎に上書きされる生成物)を追跡除去して`.gitignore`に追加。root直下43件→29件。commit(現hash`0c71ac7`)は再度`git add`漏れがあり、`git diff --cached --stat`での確認により発覚、commit(現hash`a35ca10`)で修正。
- (15) Akio指摘「`coil_01_1.gm3d`は失敗するのでは」を受け、`apps/optimize_mesh_size.py`(15段階のメッシュサイズ)と`./build/command/advcad`直接実行(メッシュサイズ1.0で4496三角形・0.025で41237三角形、いずれも`exit=0`)の両方で検証し、失敗しない事を確認。`dev/test_regression.py`の対象一覧に追加し19/19→20/20成功を確認、`CLAUDE.md`の該当箇所も更新。commit(現hash`1943e06`)。
- (16) 未pushだったtag 27件をAkio確認の上`git push --force origin --tags`で反映(17件が実際に更新、残り10件は漏洫commit以前の為ローカル・リモート一致で無変更)。push後、全remote refのtreeを走査し`advcad_gpg_secret.asc`が一切現れない事を確認。
- (17) tag `v0.15.10-secret-purge-and-cleanup`を作成しpush。

## 2. 残課題 (着手可能、緊急度の高い順)

| ID | 悪い症状の平易な説明 | それでどう困っているかの説明 | 解決策又はその糸口 |
|----|---|---|---|
| 2-1 | `docs/AutoDebugAgentsImplementPlan.md`・`docs/chat_gpt_summary/`が本セッション中に出現したが、作成者・目的が不明 | 内容を把握しないまま存在しており、既存文書との重複・矛盾があるか判断できない | 中身を読み、既存の`docs/DEBUG_META_KNOWLEDGE.md`等と重複が無いか確認する |
| 2-2 | `scripts/visualize_face7.py`と`dev/visualize_face7.py`が同一内容の重複(`*.md`整理中の副次的発見、未対応のまま) | 2-7(`optimize_mesh_size.py`)と同型のSSOT違反が残っている | 片方を正本と決め統合する |
| 2-3 | `sample/branch_pipe.gm3d`が未追跡のまま存在し、用途未確認 | 意図的な追加か作業中の残骸か不明 | 内容を読み、回帰確認対象に加えるべきか判断する |
| 2-4 | `ppa/create_ppa.sh`・`deb/build_deb.sh`は文法チェックと部分的なdry-runのみで、実際の`debuild -S -sa`・`dpkg-deb --build`は未実行 | 本番のパッケージビルドで初めて不具合が出る可能性が残る | Launchpad/dpkg環境で実際に一度通す |

## 3. 保留 (着手の前提が揃っていない。緊急度とは独立)

| ID | 事項 | 緊急度 | 保留理由 | 素案 (確定ではない) |
|----|---|---|---|---|
| 3-1 | `CHAT_OPERATIONS.md`・`PROJECT_01.md`・`TODO.md`・`prompts/`が本セッション終了時点でも未追跡(`git status`で確認済み) | 低〜中 | これらをリポジトリに追跡させるべきか、意図的に外部管理のままにするかの方針が未確定 | Akioに方針を確認し、追跡する場合は1回のcommitでまとめて追加する |

## 4. 今後の判断に影響する確定事項

- (4-1) SSOTを徹底し、重複解消の手段としてsymbolic linkは使わない(Akio指示、2-7・`*.md`整理・非`*.md`整理の全てに適用)。
- (4-2) 「参照が見当たらない＝不要」と機械的に判断しない。実行環境・利用手段(Windows主要実行・WSL2のOpenGL制約・pipenv環境構築)はAkio自身にしか分からない事があり、削除前に必ず確認する(Akio訂正、Turn単位で3件の誤判断を指摘された)。
- (4-3) 複数の実装案を並べてAkioに選ばせず、トレードオフを判断した上で1案のみ提案する(CHAT_OPERATIONS既定原則。本セッション中に複数回、選択肢提示を撤回し1案化した)。
- (4-4) 応答は必ず時刻計測→Turn見出しの順で始め、他のtool呼び出しを先に行わない(Akioより計4回指摘。徹底出来ていなかった)。
- (4-5) 不可逆操作(git履歴書き換え・force push)であっても、漏洩鍵のような重大な事案では「不可逆だから」を保留の口実にしない(Akio指示)。ただし実行前に影響範囲(全branch・全tagの到達可能性)を確認する事は省略しない。
- (4-6) `git mv`後にファイル内容を編集した場合、`git add`をやり直さないとstageされない。commit前に`git diff --cached --stat`で実差分を目視確認してからcommitする(本セッションで同型のミスを2回犯し確立)。
- (4-7) debhelper形式の`debian/`(PPA向け、`control`・`rules`・`compat`・`changelog`)と`DEBIAN/usr`形式の`debian/`(直接`.deb`向け、`dpkg-deb --build`)は別物であり、混同しない。

## 5. 成果物

- `CLAUDE.md` (改定: 成功率20/20・Windows主要環境・`benchmark.cpp`再ビルド手順・`docs/DEBUG_META_KNOWLEDGE.md`参照・`dev/test_regression.py`のpath修正記載の是正 等)
- `TODO.md` (更新、複数回。本セッション終了時点で未commit)
- `docs/DEBUG_META_KNOWLEDGE.md` (新規: advcadデバグの一般則7点・段階的デバグ手順・累積再試行の打ち切り基準)
- `CHANGELOG.md` (改定: `MODERNIZATION.md`統合・Migration Guide復元・`benchmark.cpp`再ビルド手順のpath修正)
- `GOOD_PRACTICE.md` (改定: 重複節を`CLAUDE.md`・`docs/DEBUG_META_KNOWLEDGE.md`への参照に置換)
- `MODERNIZATION.md`・`docs/CODEBASE_KNOWLEDGE.md`・`docs/CMAKE_MIGRATION_PCH_ROBUSTNESS_PLAN.md`・`my_secret_key.asc`・`reorganize.sh`・`face7_*.debug`(削除)
- `docs/THEORETICAL_MANUAL.md`・`docs/README.md`・`scripts/README.md` (陳腐化是正)
- `command/CMakeLists.txt` (`optimize_mesh_size.py`参照path修正、2回)
- `dev/test_regression.py`・`run_tests.sh` (path修正、`coil_01_1.gm3d`追加)
- `tests/regression/` (削除、`dev/test_regression.py`に一本化)
- `apps/README.md` (pipenvセットアップ手順追加)
- `dev/README.md` (Windows/OpenGL診断ツール・`reduced_tests/`の記載追加)
- `dev/face5_debug_tests/`・`dev/generated_tests/`・`dev/stress_tests/`・`dev/reduced_tests/`・`dev/test_opengl*.py`・`dev/benchmark.cpp`・`dev/benchmark` (root直下から移動)
- `ppa/` (新設: `PPA_SETUP.md`・`create_ppa.sh`・`debian/`(debhelper復元)・過去アップロード成果物一式・新鍵の公開鍵書き出し)
- `deb/` (新設: `build_deb.sh`・`debian/`(DEBIAN/usr)・`advcad_0.13.0_amd64.deb`・`copyright`・`debian_official.md`・`apt-repo/`・`setup_apt_repo.sh`・`test_package.sh`)
- `windows/` (新設: `advcad-windows-v0.13.0/`・`.zip`・`toolchain-mingw64.cmake`)
- `.gitignore` (`ppa/build/`・`face7_*.debug`追加、`/generated_tests/`の死んだ規則削除)
- git履歴(全branch・全tag): `git filter-repo`で`advcad_gpg_secret.asc`を除去、force push済み
- git tag `v0.15.10-secret-purge-and-cleanup` (新規、push済み)
