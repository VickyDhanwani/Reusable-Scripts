#!/usr/bin/env python3
"""
Identify scripts and line numbers that use Azure Automation variables.

Scans local .ps1 and .py files (or a directory) for Azure Automation variable usage.
Use this when you have exported runbooks locally, or run the PowerShell script
for direct Azure Automation account scanning.

PowerShell patterns: Get-AutomationVariable, Get-AutomationConnection, etc.
Python patterns: automationassets.get_automation_variable, get_automation_variable, etc.
"""

import re
import sys
from pathlib import Path
from typing import Iterator

# Patterns that indicate Azure Automation variable usage (case-insensitive)
PS_PATTERNS = [
    "Get-AutomationVariable",
    "Get-AutomationConnection",
    "Get-AutomationCertificate",
    "Get-AutomationCredential",
    "Get-AutomationPSCredential",
    "Get-AutomationSchedule",
]
PY_PATTERNS = [
    r"automationassets\.get_automation_variable",
    r"automationassets\.get_automation_connection",
    r"automationassets\.get_automation_credential",
    r"get_automation_variable",
    r"get_automation_connection",
    r"get_automation_credential",
]

PS_REGEX = re.compile("|".join(re.escape(p) for p in PS_PATTERNS), re.IGNORECASE)
PY_REGEX = re.compile("|".join(PY_PATTERNS), re.IGNORECASE)


def find_files(path: Path) -> Iterator[Path]:
    """Yield .ps1 and .py files under path."""
    path = Path(path).resolve()
    if not path.exists():
        raise FileNotFoundError(f"Path not found: {path}")
    if path.is_file():
        if path.suffix.lower() in (".ps1", ".py"):
            yield path
        return
    for f in path.rglob("*"):
        if f.is_file() and f.suffix.lower() in (".ps1", ".py"):
            yield f


def get_regex(ext: str) -> re.Pattern:
    return PY_REGEX if ext.lower() == ".py" else PS_REGEX


def scan_file(file_path: Path) -> list[dict]:
    """Return list of {line_number, line_content, matched} for variable usage."""
    results = []
    regex = get_regex(file_path.suffix)
    try:
        text = file_path.read_text(encoding="utf-8", errors="replace")
    except Exception as e:
        print(f"Warning: Could not read {file_path}: {e}", file=sys.stderr)
        return results
    for i, line in enumerate(text.splitlines(), start=1):
        m = regex.search(line)
        if m:
            results.append({
                "line_number": i,
                "line_content": line.strip(),
                "matched": m.group(0),
            })
    return results


def main() -> None:
    if len(sys.argv) < 2:
        print("Usage: python find_automation_variable_usage.py <path_to_folder_or_file> [output.csv]")
        print("  path: Folder to scan recursively, or a single .ps1/.py file")
        print("  output.csv: Optional. Save results to CSV.")
        sys.exit(1)

    root = Path(sys.argv[1])
    output_path = sys.argv[2] if len(sys.argv) > 2 else None

    all_results = []
    for f in find_files(root):
        for r in scan_file(f):
            all_results.append({
                "script": str(f),
                "script_name": f.name,
                "line_number": r["line_number"],
                "line_content": r["line_content"],
                "matched_pattern": r["matched"],
            })

    if not all_results:
        print("No Azure Automation variable usage found.")
        sys.exit(0)

    print(f"Found {len(all_results)} line(s) using Automation variables:\n")
    for row in all_results:
        print(f"  {row['script']}:{row['line_number']}")
        print(f"    [{row['matched_pattern']}] {row['line_content'][:80]}...")
        print()

    if output_path:
        import csv
        with open(output_path, "w", newline="", encoding="utf-8") as f:
            w = csv.DictWriter(f, fieldnames=["script", "script_name", "line_number", "line_content", "matched_pattern"])
            w.writeheader()
            w.writerows(all_results)
        print(f"Results saved to: {output_path}")


if __name__ == "__main__":
    main()
