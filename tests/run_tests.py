#!/usr/bin/env python3
"""
DevDictates - Unified Automated Test Runner.
Tests binary tree construction implementations across C, C++, Java, and Python.
Standard library only.
Author: Pragya
"""

import os
import sys
import shutil
import subprocess
from pathlib import Path


ROOT_DIR = Path(__file__).resolve().parent.parent
TESTS_DIR = ROOT_DIR / "tests"
CASES_FILE = TESTS_DIR / "test_cases" / "cases.txt"
EXPECTED_FILE = TESTS_DIR / "expected" / "expected_outputs.txt"

C_SRC = ROOT_DIR / "C" / "solution.c"
C_BIN = ROOT_DIR / "C" / "solution"
CPP_SRC = ROOT_DIR / "C++" / "solution.cpp"
CPP_BIN = ROOT_DIR / "C++" / "solution"
JAVA_SRC = ROOT_DIR / "Java" / "Main.java"
JAVA_CLASS = ROOT_DIR / "Java" / "Main.class"
PY_SRC = ROOT_DIR / "Python" / "solution.py"


def parse_blocks(file_path):
    """Parse text file containing blocks delimited by '=== TEST <name> ==='."""
    blocks = {}
    current_name = None
    current_lines = []

    with open(file_path, "r", encoding="utf-8") as f:
        for line in f:
            stripped = line.strip()
            if stripped.startswith("=== TEST ") and stripped.endswith(" ==="):
                if current_name is not None:
                    blocks[current_name] = "\n".join(current_lines).strip()
                current_name = stripped[len("=== TEST "): -len(" ===")].strip()
                current_lines = []
            else:
                current_lines.append(line.rstrip("\r\n"))

        if current_name is not None:
            blocks[current_name] = "\n".join(current_lines).strip()

    return blocks


def check_tool(binary_name, version_arg="--version"):
    """Check if a tool exists and runs without error."""
    path = shutil.which(binary_name)
    if not path:
        return False, None
    try:
        proc = subprocess.run([path, version_arg], stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=5)
        # On macOS, stub binaries might exit with error code or print "Unable to locate"
        combined = (proc.stdout + proc.stderr).decode("utf-8", errors="ignore")
        if "Unable to locate a Java Runtime" in combined or "No Java runtime present" in combined:
            return False, None
        if proc.returncode == 0:
            return True, path
    except Exception:
        return False, None
    return False, None


def setup_compilers():
    """Detect compilers, compile C, C++, and Java, and prepare run commands."""
    targets = {}

    # 1. C
    c_compiler = None
    for cc in ["clang", "gcc"]:
        ok, path = check_tool(cc)
        if ok:
            c_compiler = path
            break

    if c_compiler:
        c_sources = [str(ROOT_DIR / "C" / f) for f in ["tree.c", "reconstruction.c", "solution.c"]]
        cmd = [c_compiler, "-O2", "-Wall", "-Wextra", *c_sources, "-o", str(C_BIN)]
        res = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        if res.returncode == 0 and C_BIN.exists():
            targets["C"] = {"cmd": [str(C_BIN)], "status": "READY"}
        else:
            targets["C"] = {"status": "BUILD_FAILED", "error": res.stderr}
    else:
        targets["C"] = {"status": "SKIPPED", "reason": "No C compiler (clang/gcc) found"}

    # 2. C++
    cpp_compiler = None
    for cxx in ["clang++", "g++"]:
        ok, path = check_tool(cxx)
        if ok:
            cpp_compiler = path
            break

    if cpp_compiler:
        cmd = [cpp_compiler, "-O2", "-std=c++17", "-Wall", "-Wextra", str(CPP_SRC), "-o", str(CPP_BIN)]
        res = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        if res.returncode == 0 and CPP_BIN.exists():
            targets["C++"] = {"cmd": [str(CPP_BIN)], "status": "READY"}
        else:
            targets["C++"] = {"status": "BUILD_FAILED", "error": res.stderr}
    else:
        targets["C++"] = {"status": "SKIPPED", "reason": "No C++ compiler (clang++/g++) found"}

    # 3. Java
    javac_ok, javac_path = check_tool("javac", "-version")
    java_ok, java_path = check_tool("java", "-version")
    if javac_ok and java_ok:
        res = subprocess.run([javac_path, str(JAVA_SRC)], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        if res.returncode == 0 and JAVA_CLASS.exists():
            targets["Java"] = {"cmd": [java_path, "-cp", str(ROOT_DIR / "Java"), "Main"], "status": "READY"}
        else:
            targets["Java"] = {"status": "BUILD_FAILED", "error": res.stderr}
    else:
        targets["Java"] = {"status": "SKIPPED", "reason": "Java runtime/JDK not available on system"}

    # 4. Python
    py_path = sys.executable
    if py_path and PY_SRC.exists():
        targets["Python"] = {"cmd": [py_path, str(PY_SRC)], "status": "READY"}
    else:
        targets["Python"] = {"status": "SKIPPED", "reason": "Python runtime or script not found"}

    return targets


def normalize_output(text):
    """Normalize output lines for comparison."""
    lines = [line.strip() for line in text.strip().splitlines() if line.strip()]
    return "\n".join(lines)


def run_single_test(cmd, input_text):
    """Run target executable with input_text on stdin."""
    try:
        proc = subprocess.run(
            cmd,
            input=input_text,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            timeout=5
        )
        return proc.stdout.strip(), proc.stderr.strip(), proc.returncode
    except subprocess.TimeoutExpired:
        return "", "Execution timed out (5s limit)", -1
    except Exception as e:
        return "", str(e), -1


def match_output(actual, expected):
    """Compare actual output against expected output."""
    act_lines = [l.strip() for l in actual.splitlines() if l.strip()]
    exp_lines = [l.strip() for l in expected.splitlines() if l.strip()]

    if not act_lines:
        return False, "No output produced"

    # If expecting ERROR:
    if exp_lines and exp_lines[0].startswith("STATUS: ERROR"):
        if act_lines[0].startswith("STATUS: ERROR"):
            return True, ""
        return False, f"Expected STATUS: ERROR, got: {act_lines[0]}"

    # If expecting SUCCESS:
    if len(act_lines) < len(exp_lines):
        return False, f"Insufficient output lines: expected {len(exp_lines)}, got {len(act_lines)}"

    # Check STATUS, PREORDER, POSTORDER
    act_dict = {}
    for line in act_lines:
        if ":" in line:
            k, v = line.split(":", 1)
            act_dict[k.strip()] = v.strip()

    exp_dict = {}
    for line in exp_lines:
        if ":" in line:
            k, v = line.split(":", 1)
            exp_dict[k.strip()] = v.strip()

    for k in ["STATUS", "PREORDER", "POSTORDER"]:
        if k in exp_dict:
            if k not in act_dict:
                return False, f"Missing key '{k}' in output"
            if act_dict[k] != exp_dict[k]:
                return False, f"Mismatch in {k}:\n  Expected: '{exp_dict[k]}'\n  Actual:   '{act_dict[k]}'"

    return True, ""


def main():
    print("=" * 70)
    print("DevDictates - Unified Multi-Language Test Runner")
    print("Algorithm: Binary Tree Construction (Preorder + Postorder Traversals)")
    print("=" * 70)

    if not CASES_FILE.exists() or not EXPECTED_FILE.exists():
        print(f"Error: Missing test cases or expected files in {TESTS_DIR}")
        sys.exit(1)

    cases = parse_blocks(CASES_FILE)
    expected = parse_blocks(EXPECTED_FILE)

    common_keys = [k for k in cases if k in expected]
    print(f"Loaded {len(common_keys)} test case fixtures.\n")

    targets = setup_compilers()

    overall_results = {}
    total_failures = 0

    for lang, info in targets.items():
        print(f"[{lang}] Setup status: {info['status']}")
        if info['status'] != "READY":
            reason = info.get('reason') or info.get('error', 'unknown error')
            print(f"  -> Skipping {lang}: {reason}\n")
            overall_results[lang] = ("SKIPPED", f"({reason})")
            continue

        cmd = info["cmd"]
        passed_count = 0
        failed_count = 0

        print(f"  Running tests for {lang}...")
        for name in common_keys:
            in_text = cases[name]
            exp_text = expected[name]

            actual_out, actual_err, retcode = run_single_test(cmd, in_text)
            ok, err_msg = match_output(actual_out, exp_text)

            if ok:
                passed_count += 1
            else:
                failed_count += 1
                total_failures += 1
                print(f"    FAIL [{name}]: {err_msg}")
                if actual_err:
                    print(f"         stderr: {actual_err}")

        if failed_count == 0:
            print(f"  -> {lang}: ALL {passed_count} TESTS PASSED!\n")
            overall_results[lang] = ("PASS", f"({passed_count}/{len(common_keys)} passed)")
        else:
            print(f"  -> {lang}: {failed_count} TESTS FAILED ({passed_count}/{len(common_keys)} passed)\n")
            overall_results[lang] = ("FAIL", f"({failed_count} failed)")

    print("=" * 70)
    print("FINAL SUMMARY REPORT")
    print("=" * 70)
    for lang, (res, details) in overall_results.items():
        print(f"{lang:<8}: {res:<8} {details}")
    print("=" * 70)

    if total_failures > 0:
        print("RESULT: FAILED (one or more test suites failed)")
        sys.exit(1)
    else:
        print("RESULT: SUCCESS (all active test suites passed)")
        sys.exit(0)


if __name__ == "__main__":
    main()
