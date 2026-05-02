# TODO

リポジトリに散在する TODO・残課題を集約したリスト。出典を併記しているので、詳細は各ドキュメント・ファイルを参照のこと。

最終確認時の挙動: 回帰テスト 19/19 通過 (`mesh_size=1.0`)。ただし `air_practice.gm3d` / `air_up2_top_01.gm3d` は過大メッシュで形状が潰れているだけで、推奨メッシュサイズでは退化警告が大量に出る。

---

## 1. ソースコード内 TODO

### 1.1 ギャップ解析の未実装
- 場所: `WH/geometry_analyzer.cc:158`
- 内容: `computeMinimumGap()` が未実装。暫定で `computeMinimumFeatureSize() * 0.5` を返す。
- 影響: 面間のギャップに基づくメッシュサイズ推奨値が正確でない。
- 関連: `DEGENERATE_TRIANGLE_FIX_PLAN.md` Phase 0。

### 1.2 厚み解析の未実装
- 場所: `WH/geometry_analyzer.cc:164`
- 内容: `computeMinimumThickness()` が未実装。暫定で `computeMinimumFeatureSize() * 0.1` を返す。
- 影響: 薄肉領域の検出ができず、推奨メッシュサイズが甘くなる可能性。
- 関連: `DEGENERATE_TRIANGLE_FIX_PLAN.md` Phase 0。

---

## 2. メッシュ生成アルゴリズムの残課題

### 2.1 advancing front 失敗 (2 モデル)
- 対象: `sample/shaft/air_practice.gm3d`, `sample/shaft/air_up2_top_01.gm3d`
- 症状: 推奨メッシュサイズ域で `afront2d.cc:884` のアサーション、または大量の退化警告。
- 根本原因: 重複頂点・ゼロ長エッジ等の重度な幾何退化。
- 出典: `CLAUDE.md` "Remaining Debug Targets"。
- 備考: 現在の回帰テスト (mesh=1.0) では rc=0 になるが、これは過大メッシュで形状が崩れているため(11~17 三角形しか生成されない)。実質的には未解決。

### 2.2 退化形状での警告ノイズ
- 症状: 推奨メッシュサイズ付近で以下の警告が多発:
  - `Normalizing zero-length vector - returning unit X vector`
  - `Division by zero in 2D vector operation - returning zero vector`
  - `Degenerate triangle - computed radius=-nan` (フォールバック半径使用)
  - `Nearly collinear triangle detected (area=0, ...)`
- 影響: クラッシュはしないが、低品質三角形を黙って通している可能性。
- 推奨: フォールバック発動箇所を計測し、入力幾何の前処理(重複点マージ等)で発生頻度を下げる。

---

## 3. PCH 検証パイプライン (CMAKE_MIGRATION_PCH_ROBUSTNESS_PLAN.md)

### 3.1 PchReader を回帰テストに統合 (Task 5 / PENDING)
- 既存の `scripts/PchReader.py` (連結性整合性 + Mixed triangle 検出) を `dev/test_regression.py` から呼び出す。
- 現状: 回帰テストはファイルサイズ > 0 でしか成否を判定していない。

### 3.2 検証強制化 (Task 6 / PENDING)
- 連結性異常 (例: Face 7 の Mixed triangle) を回帰テストの失敗扱いにする。
- 依存: Task 5 完了。

### 3.3 既存 .pch ファイルの検証 (Task 7 / PENDING・LOW)
- `tests/data/shaft/` 配下の既存 `.pch` を `PchReader.py` で全件チェック。

### 3.4 ディレクトリ構造の整合 (Note)
- プラン上のターゲットは `src/WH/` + `src/command/`。
- 実体は `WH/` + `command/` (フラット構成)。すでにビルドは通っているので、プラン文書をリポジトリ実態に合わせて更新するか、移設するかの判断が必要。

---

## 4. 失敗境界マッピング (MESH_FAILURE_ANALYSIS_PLAN.md)

`dev/mesh_failure_analyzer.py` は存在するが、以下のアウトプットがリポジトリに揃っていない。

- [ ] 全 19 モデル × 12 メッシュ係数 (192 ケース) のフルマトリクス実行。
- [ ] `mesh_failure_report.md` の生成 (失敗パターン分類・安全範囲・遷移点)。
- [ ] `mesh_failure_results.json` の出力 (機械可読の生データ)。

失敗カテゴリ予定: `ZERO_LENGTH_VECTOR`, `ASSERTION_FAILURE`, `MIXED_TRIANGLES`, `TIMEOUT`, `MEMORY_ERROR`, `OTHER`。

---

## 5. ドキュメントの更新ずれ

PLAN 系・CLAUDE.md の数値が実態と乖離しているので追従が必要。

| 文書 | 記載値 | 実態(本ブランチで確認) |
|---|---|---|
| `CLAUDE.md` | 89.5% (17/19) | 19/19 通過 (mesh=1.0)。ただし 2 モデルは過大メッシュで実質失敗。 |
| `ZERO_VECTOR_FIX_PLAN.md` | 68.4% (13/19) | 既に修正済み (v0.12.1)。文書は履歴扱いに。 |
| `MESH_FAILURE_ANALYSIS_PLAN.md` | 84.2% (16/19) | 同上、文書中の母数も古い。 |
| `dev/test_regression.py` | `project_root = /home/miyoshi/...` ハードコード | 現環境では動かない。`Path(__file__).resolve().parent.parent` 等に置換が必要。 |

- [ ] CLAUDE.md "Current Success Metrics" を最新の検証結果に更新。
- [ ] `dev/test_regression.py` の `project_root` を環境非依存に修正。
- [ ] 解決済み PLAN (`ZERO_VECTOR_FIX_PLAN.md` 等) を `docs/history/` 等に移動するか、冒頭に "RESOLVED" バッジを付与。

---

## 6. 完了済み (参考)

実装が確認できた項目。今後 PLAN 文書から削除/アーカイブして良い。

- ✅ ゼロ長ベクトル除算保護 (`WH/space2d_inline.cc:109`) — v0.12.1
- ✅ ロバスト CDT のドメイン ID 割当 (`WH/robust_cdt.cc`) — v0.12.3
- ✅ 小スケール幾何の自動ロバスト CDT 切替 — v0.12.4
- ✅ Face 5 の 6 ノードしきい値修正 (`> 6` → `>= 6`) — v0.12.6
- ✅ メッシュサイズ推奨範囲の警告出力 (CLI 起動時)
- ✅ 退化三角形検出と circumradius フォールバック (`DEGENERATE_TRIANGLE_FIX_PLAN.md` Phase 1)
- ✅ `WH_GeometryAnalyzer` 本体 (`WH/geometry_analyzer.cc`) — ギャップ/厚み解析を除く
- ✅ `scripts/PchReader.py` (CMAKE プラン Task 4)
- ✅ CMake ビルド成立 (CMAKE プラン Task 1〜3 相当、ただしレイアウトは `WH/` + `command/`)
