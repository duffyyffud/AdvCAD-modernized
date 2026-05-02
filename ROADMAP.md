# ROADMAP

このプロジェクトの開発方針と中長期計画。短期の具体的タスクは [`TODO.md`](TODO.md) を参照。

## トップレベルの意図(優先順位)

1. **【Priority 1】ロバスト化** ─ advcad を堅牢にすることが最大の目的。退化幾何・極端なメッシュサイズ・小スケール座標などの入力で、クラッシュ・誤った成功・低品質出力を起こさず、根本原因を解析・修復できる状態にする。
2. **【Priority 2】GUI 中心のユーティリティ整備** ─ `.gm3d` 編集 → メッシュ生成 → 結果可視化 の一連の作業を、CLI に頼らず GUI で完結できるようにする。エンドユーザーの実用性を最優先。
3. **【Priority 3】コードの現代化** ─ C++20 への移行・追加の constexpr 最適化・テンプレート特殊化等は **優先度を下げる**。既存の C++17 ベースで上記 Priority 1/Priority 2 が阻害されない範囲で必要に応じて行う。

---

## 現状(2026-05 時点、`claude/verify-repo-behavior-YcNzN` で検証)

- 回帰テスト: 19/19 が `rc=0` で通過(`mesh=1.0`)。
- 実効カバレッジ: 17/19。`shaft/air_practice.gm3d` と `shaft/air_up2_top_01.gm3d` は推奨より ~700 倍粗いメッシュで形状崩壊しているため、実質失敗。
- 旧来のアサーションクラッシュは「警告 + フォールバック」に置き換わっており、退化形状でも完走する代わりに、低品質出力を黙って通している可能性がある。
- GUI: `apps/gm3d_editor.py` が PyQt5 ベースで存在(テキスト編集 + 構文ハイライト主体)。可視化や AdvCAD 実行との統合は弱い。

---

## Phase 1 ─ ロバスト化の即効性向上(Priority 1、短期)

`TODO.md` の未消化項目のうち効果の出やすいものを優先消化する。

- [ ] **`WH/geometry_analyzer.cc` の本実装** ([TODO.md §1.1, §1.2](TODO.md))
  - `computeMinimumGap` / `computeMinimumThickness` のスタブを置き換え、推奨メッシュサイズの精度を底上げ。
  - 現在の警告 `WARNING: Mesh size N exceeds recommended maximum …` の根拠が正確になる。
- [ ] **PCH 検証を回帰テストに統合** ([TODO.md §3.1, §3.2](TODO.md))
  - `scripts/PchReader.py` の Mixed triangle / 連結性チェックを `dev/test_regression.py` から呼び出し、検証失敗を回帰失敗として扱う。
  - これによって "rc=0 だが低品質出力" を見逃さなくなる。
- [ ] **`dev/test_regression.py` の環境非依存化** ([TODO.md §5](TODO.md))
  - ハードコードされた `/home/miyoshi/...` を撤去し、リポジトリルート相対に。CI 化の前提整備。
- [ ] **`air_practice.gm3d` / `air_up2_top_01.gm3d` の根本原因解析** ([TODO.md §2.1](TODO.md))
  - `afront2d.cc:884` の退化条件を再現する最小ケースを作り、advancing front の失敗を構造的に分類する。
  - 修正案 (重複頂点マージ・ゼロ長エッジ除去等の前処理) を 1 案以上実装。

**Phase 1 完了基準**: 推奨メッシュサイズ域でも 19/19 が "rc=0 + PCH 検証通過" になる、もしくは失敗時に明確なエラーで止まる(黙って低品質出力を返さない)。

---

## Phase 2 ─ ロバスト化の体系化と GUI 拡張(Priority 1 + Priority 2、中期)

ロバスト化の網羅性を高めつつ、GUI ユーティリティの土台を作る。

### Priority 1: ロバスト化(続き)

- [ ] **失敗境界マトリクスの完成** ([TODO.md §4](TODO.md))
  - `dev/mesh_failure_analyzer.py` で 19 モデル × 12 メッシュ係数の全 192 ケースを実行し、`mesh_failure_report.md` / `mesh_failure_results.json` を生成・コミット。
  - 失敗パターンを `ZERO_LENGTH_VECTOR` / `ASSERTION` / `MIXED_TRIANGLES` / `TIMEOUT` 等に分類。
- [ ] **退化警告の定量化と低減** ([TODO.md §2.2](TODO.md))
  - フォールバック発動箇所(zero-vector / div-by-zero / nan radius / collinear)に発動カウンタを設置。
  - 現在の発動頻度を計測し、入力前処理(重複点マージ等)で X% 以上削減することを目標とする。
- [ ] **CI 整備**
  - GitHub Actions 等で `dev/test_regression.py` + PCH 検証を毎 PR で実行。

### Priority 2: GUI ユーティリティ

- [ ] **`apps/gm3d_editor.py` の AdvCAD 連動**
  - 編集中の `.gm3d` をその場でメッシュ生成し、結果三角形数や警告を表示。
  - Mesh size スライダで対話的に試行錯誤できる。
- [ ] **PCH 可視化ツールの統合**
  - 既存の `scripts/visualize_face7.py`(face 単位)と `dev/visualize_face7.py` を統合し、PCH ファイル全体を 3D ビューイング可能に。
  - ツールキット候補: PyVista / VTK / Open3D。
- [ ] **メッシュ品質ダッシュボード**
  - PchReader 出力を元に、最小角度 / アスペクト比 / 退化三角形数 を表示。

**Phase 2 完了基準**: GUI から `.gm3d` 編集 → メッシュ生成 → 品質確認 が CLI なしで完結する。

---

## Phase 3 ─ 統合ワークフローの確立(Priority 2 強化、長期)

エンドユーザーが CLI を意識せずに使えるツールチェーンを整える。

- [ ] **統合 GUI アプリケーション**
  - 単一ウィンドウで `.gm3d` 編集 / プレビュー / メッシュ生成 / PCH 可視化 / 品質確認 を完結。
- [ ] **バッチ処理 GUI**
  - 複数モデル + 複数メッシュサイズの一括処理と結果比較。
- [ ] **`apps/advcad_auto.py` の GUI フロントエンド化**
  - メッシュサイズ最適化を進捗バー付きで実行。
- [ ] **エラー可視化**
  - PchReader が検出した不整合(Mixed triangle 等)を 3D ビュー上で問題箇所として強調表示。

**Phase 3 完了基準**: README から CLI コマンドのセクションを引かなくても、エンドユーザーが GUI だけで実用作業を完了できる。

---

## Backlog(Priority 3、優先度低)

`CHANGELOG.md:121` "Future Roadmap" と `MODERNIZATION.md:190` "Future Enhancements" に列挙されていた項目。**Priority 1/Priority 2 を阻害しない範囲で、機会があれば**着手する。スケジュール化はしない。

- C++20 features の採用 (concepts, modules)
- 追加 constexpr 最適化
- テンプレート特殊化の改善
- 例外安全性保証の強化
- (完了) 解決済み PLAN 文書のアーカイブ整理 → `PLANS.md` に統合済み

---

## 文書間の役割分担

| 文書 | 守備範囲 |
|---|---|
| `ROADMAP.md` (本文書) | 中長期方針と Phase 構造、優先度の明示 |
| `TODO.md` | 短期の未消化タスク(出典付き) |
| `CHANGELOG.md` | 過去のリリース履歴 |
| `CLAUDE.md` | コードベースガイド(現状認識・デバッグ手法) |
| 個別 `*_PLAN.md` | 過去・進行中の作業計画(履歴的価値) |

ROADMAP は方向、TODO は次の一手、CHANGELOG は過去ログ ─ という関係。
