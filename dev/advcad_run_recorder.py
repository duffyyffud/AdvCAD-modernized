#!/usr/bin/env python3
"""advcad を1回実行し、実行契約の記録7項目を記録する。

記録7項目: 終了コード、標準出力、標準エラー、実行時間、最大メモリ、
出力ファイルの存在、出力ファイルのサイズ。
根拠: docs/AutoDebugAgentsImplementPlan.md 第5章 Step 1、第11章フェーズ1。
タイムアウト・sanitizer・ログ回収基盤・gm3d独立パーサはこのスクリプトの対象外（別タスク）。
"""

import subprocess
import sys
import tempfile
from pathlib import Path


def parse_time_report(time_report_text):
    """/usr/bin/time -v の出力から実行時間と最大メモリを取り出す"""
    elapsed_time = None
    max_memory_kb = None
    for line in time_report_text.splitlines():
        line = line.strip()
        if line.startswith("Elapsed (wall clock) time") and "): " in line:
            elapsed_time = line.split("): ", 1)[1].strip()
        elif line.startswith("Maximum resident set size") and "): " in line:
            max_memory_kb = line.split("): ", 1)[1].strip()
    return elapsed_time, max_memory_kb


def run_advcad_once(geometry_file, patch_file, patch_size, use_pcm):
    """advcad を1回実行し、記録7項目を辞書で返す"""
    project_root = Path(__file__).resolve().parent.parent
    advcad_exe = project_root / "build" / "command" / "advcad"

    if not advcad_exe.exists():
        raise FileNotFoundError("advcad実行ファイルが見つからない: " + str(advcad_exe))

    advcad_command = [str(advcad_exe), geometry_file, patch_file, str(patch_size)]
    if use_pcm:
        advcad_command.append("-pcm")

    time_report_file = tempfile.NamedTemporaryFile(mode="w", suffix=".time", delete=False)
    time_report_path = time_report_file.name
    time_report_file.close()

    time_command = ["/usr/bin/time", "-v", "-o", time_report_path] + advcad_command

    result = subprocess.run(time_command, capture_output=True, text=True)

    time_report_text = Path(time_report_path).read_text()
    Path(time_report_path).unlink()

    elapsed_time, max_memory_kb = parse_time_report(time_report_text)

    output_path = Path(patch_file)
    output_exists = output_path.exists()
    output_size_bytes = output_path.stat().st_size if output_exists else None

    return {
        "exit_code": result.returncode,
        "stdout": result.stdout,
        "stderr": result.stderr,
        "elapsed_time": elapsed_time,
        "max_memory_kb": max_memory_kb,
        "output_exists": output_exists,
        "output_size_bytes": output_size_bytes,
    }


def print_record(record):
    """記録7項目を人間可読な標準出力へ書き出す"""
    print("終了コード:", record["exit_code"])
    print("実行時間:", record["elapsed_time"])
    print("最大メモリ(KB):", record["max_memory_kb"])
    print("出力ファイルの存在:", record["output_exists"])
    print("出力ファイルのサイズ(バイト):", record["output_size_bytes"])
    print("標準出力:")
    print(record["stdout"])
    print("標準エラー:")
    print(record["stderr"])


def main():
    if len(sys.argv) not in (4, 5):
        print("使い方: advcad_run_recorder.py <geometry_file> <patch_file> <patch_size> [-pcm]")
        sys.exit(1)

    geometry_file = sys.argv[1]
    patch_file = sys.argv[2]
    patch_size = sys.argv[3]
    use_pcm = False
    if len(sys.argv) == 5:
        if sys.argv[4] != "-pcm":
            print("使い方: advcad_run_recorder.py <geometry_file> <patch_file> <patch_size> [-pcm]")
            sys.exit(1)
        use_pcm = True

    record = run_advcad_once(geometry_file, patch_file, patch_size, use_pcm)
    print_record(record)


if __name__ == "__main__":
    main()
