# Implementation Plans

個別の技術的作業計画を集約した文書。元は 4 つの `*_PLAN.md` に分散していた内容を統合。

役割分担:
- [`ROADMAP.md`](ROADMAP.md) — 中長期方針と優先度(なぜやるか)
- [`TODO.md`](TODO.md) — 短期タスク(次に着手するもの)
- 本文書 (`PLANS.md`) — 個別の実装計画の技術詳細(どうやるか)
- [`CHANGELOG.md`](CHANGELOG.md) — 過去の変更履歴

---

## 索引

| # | 計画 | 状態 | 関連バージョン |
|---|---|---|---|
| 1 | [CMake 移行 & PCH 検証統合](#1-cmake-migration--pch-validation) | 進行中(Task 4 完了、5〜7 未消化) | — |
| 2 | [退化三角形ハンドリング](#2-degenerate-triangle-handling) | 大部分実装済み、未完部分あり | v0.12.4〜v0.12.6 |
| 3 | [失敗境界マッピング](#3-mesh-failure-boundary-mapping) | ツール存在、フル実行は未 | — |
| 4 | [ゼロ長ベクトル除算修正](#4-zero-length-vector-division-fix) | 完了 | v0.12.1 |

---

## 1. CMake Migration & PCH Validation

### 目的
従来の Makefile から CMake への移行と、PCH ファイル検証によるメッシュ整合性チェックの導入。

### 現状
- ✅ Task 1: フラットディレクトリ構造の確立 — `WH/` + `command/` で運用中(プラン当初の `src/WH/` ターゲットとは異なるが実用上 OK)
- ✅ Task 2: CMake 設定の整備 — `cd build && make -j4` でビルド成立
- ✅ Task 3: shaft モデルでの動作確認 — 回帰テスト 19/19 通過
- ✅ Task 4: `PchReader.py` 作成 — `scripts/PchReader.py` に存在(連結性整合性 + Mixed triangle 検出)
- ⏳ Task 5: PchReader を回帰テストへ統合
- ⏳ Task 6: 検証強制化(Mixed triangle 等を回帰失敗とみなす)
- ⏳ Task 7: 既存 `.pch` ファイルの検証(LOW)

### 残タスク詳細

#### Task 5: PchReader を `dev/test_regression.py` から呼び出す
- 各モデルのメッシュ生成後に `PchReader.validate(out_path)` を呼び、結果を回帰結果に統合。
- 現状の判定基準(`rc==0 && size>0`)に「PCH 検証 OK」を追加。

#### Task 6: 検証失敗を回帰失敗扱いにする
- Task 5 の結果を `passed` フラグに反映。
- 検証カテゴリ別(Mixed triangle / 連結性 / 退化)に分類して報告。

#### Task 7: 既存 PCH の検証
- `tests/data/shaft/` 配下を `PchReader.py` で全件チェック。検出された問題はチケット化。

### 成功基準
- CMake ビルドが安定動作。
- 全 shaft モデルが妥当な `.pch` を生成。
- 回帰テストが Mixed triangle を自動検出。
- PCH 検証が回帰スイートに組み込まれている。

---

## 2. Degenerate Triangle Handling

### 目的
微小メッシュサイズや退化幾何で発生する `WH_lt(0, _radiusOfCircle)` 等のアサーション失敗を防ぎ、低品質出力を黙って返さないようにする。

### 現状
- ✅ Phase 0(メッシュサイズ妥当性検証): `WH/geometry_analyzer.cc` 実装済み。`WH_GeometryAnalyzer::analyze()` / `recommendMeshSize()` / 起動時の "Recommended mesh size range" 出力が動作。
  - ⚠️ **未完**: `computeMinimumGap()` (`:158`) と `computeMinimumThickness()` (`:164`) はスタブ。
- ✅ Phase 1(退化三角形検出と回避): 実装済み。`Degenerate triangle - computed radius=-nan` 警告 + フォールバック半径、`Nearly collinear triangle detected` 警告が出力される。
- ✅ Phase 2(パイプライン統合): `command/advcad.cc` 起動時にメッシュサイズ範囲を出力し、範囲外では `WARNING: Mesh size N exceeds recommended maximum …` を発出。
- 🟡 Phase 3(品質コントロール): post-triangulation の quality report は未実装の見込み(要確認)。アスペクト比・最小角度・退化数の集計が未整備。
- 🟡 Phase 4(`apps/optimize_mesh_size.py` 改修): スクリプト自体は存在。geometry analyzer の正確化(Phase 0 の gap/thickness 完成)に追従する形で見直し要。

### 残タスク詳細

#### Phase 0 残作業: gap / thickness 解析の本実装
- `computeMinimumGap(body)`: 面間ギャップの最小値を測定。実装案:
  - 面ペアの bounding box が近接しているものに絞り込み、面間の符号付き距離の最小値を取る。
- `computeMinimumThickness(body)`: 薄肉領域の検出。実装案:
  - 法線が反対向きに近い面ペアの間隔から、最小厚さを推定。
- これらが正確になることで、推奨メッシュサイズの精度が上がり、現状の "WARNING: Mesh size N too large" が信頼できる根拠を持つ。

#### Phase 3: メッシュ品質レポート
```cpp
struct MeshQualityReport {
    int totalTriangles;
    int degenerateTriangles;
    double minimumAngle;
    double maximumAngle;
    double averageAspectRatio;
    double minimumArea;
};
```
- メッシュ生成後に集計し、警告閾値を超える場合はサマリを stderr に出力。
- `PchReader.py` 側でも同等の解析を提供できるようにする(検証統合時の入力)。

#### Phase 4: optimize_mesh_size.py の追従
- 新しい geometry analyzer の出力(gap / thickness)を読み取り、推奨メッシュサイズの算出ロジックを更新。
- 数値精度限界 (`bbox_diagonal * 1e-6`) を下限としてクランプ。

### 成功基準
- 不適切なメッシュサイズで自動調整 + 明示警告が出る。
- 退化三角形でクラッシュしない(維持)。
- 退化が発生した場合、品質レポートに件数が記録される。
- メッシュ品質が全テストケースで一定基準を満たす。

---

## 3. Mesh Failure Boundary Mapping

### 目的
通過モデルの破綻境界を体系的に把握し、アルゴリズムの弱点と再現可能なエッジケースを特定する。

### 方法論
モデルごとの最適メッシュサイズに対する **相対係数** で複数サイズをテスト:

```
factors = [0.01, 0.05, 0.1, 0.25, 0.5, 0.75, 1.0, 1.5, 2.0, 3.0, 5.0, 10.0]
test_size = optimal_mesh_size(model) × factor
```

| 係数帯 | 意図 |
|---|---|
| 0.01〜0.1 | 数値精度の限界(極細) |
| 0.25〜0.75 | 最適より細かい(ロバスト性) |
| 1.0 | ベースライン |
| 1.5〜3.0 | 最適より粗い(特徴捕捉) |
| 5.0〜10.0 | 極粗(細部欠落) |

### 現状
- ✅ ツール `dev/mesh_failure_analyzer.py` は存在。
- ⏳ 19 モデル × 12 係数 = 228 ケースのフル実行が未実施(プラン当初は 16 モデル × 12 = 192)。
- ⏳ `mesh_failure_report.md` 未生成。
- ⏳ `mesh_failure_results.json` 未生成。

### 残タスク詳細

#### フル実行と分類
- 失敗カテゴリ:
  - `ZERO_LENGTH_VECTOR` — 2D 操作の 0 除算
  - `ASSERTION_FAILURE` — その他の `WH_ASSERT` 失敗(箇所付き)
  - `MIXED_TRIANGLES` — Face 7 系
  - `TIMEOUT` — メッシュが細かすぎて爆発
  - `MEMORY_ERROR` — 割当失敗
  - `OTHER` — 未分類
- 各モデルについて SUCCESS↔FAIL の遷移点を記録し、安全範囲を文書化。

#### レポート生成
- マークダウン: パターンサマリ、安全範囲、遷移分析、改善提案。
- JSON: 機械可読の生データ。

#### CLI
```
python3 dev/mesh_failure_analyzer.py --model test_1.gm3d
python3 dev/mesh_failure_analyzer.py --all
python3 dev/mesh_failure_analyzer.py --report
```

### 成功基準
- 全モデルの失敗境界が文書化されている。
- 3 種以上の失敗パターンが特定されている。
- 各失敗モードに再現テストケースがある。
- 安全運用範囲が文書化されている。

---

## 4. Zero-Length Vector Division Fix

### 状態: ✅ RESOLVED (v0.12.1)

### 元の問題
- `WH_ne(ratio, 0.0)` アサーションが `WH/space2d_inline.cc:109` で発生。
- 6 モデル(`test_3/4/7.gm3d`, `shaft/air_practice.gm3d`, `shaft/air_up2_top_01.gm3d`, `shaft/cyclic_mag_body_01.gm3d`)で再現。
- 原因: 退化幾何 → ゼロ長ベクトル → 2D 除算で 0 除算 → アサーション。

### 解決策
- 3D 側 (`space3d_inline.cc`) と同等のゼロ長保護を 2D 側にも実装。
- ゼロ長検出時は `WARNING: Division by zero in 2D vector operation - returning zero vector` を出力してゼロベクトルを返す。

### 結果
- v0.12.1 リリース時点で 68.4% → ~84.2% に改善。
- 後続の修正(Domain ID v0.12.3, Robust CDT 拡張 v0.12.4, Face 5 v0.12.6)と組み合わさり、現在 19/19 通過(実効 17/19)。
- 実装箇所: `WH/space2d_inline.cc` のゼロ長チェック追加。

### 残された関連課題
- ゼロ長ベクトルや 0 除算は警告 + フォールバックで通っているが、**発動頻度自体は計測されていない**。`TODO.md §2.2` および `ROADMAP.md` Phase 2 で、フォールバック発動箇所のカウンタ設置と入力前処理による削減を予定。
