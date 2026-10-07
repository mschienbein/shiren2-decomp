#!/usr/bin/env python3
"""Write an objdiff-format progress report (report.json) for decomp.dev.

    python3 ci/progress_report.py --version jp --output report.json

decomp.dev ingests an objdiff `Report` (objdiff-core/protos/report.proto, protobuf JSON mapping:
uint64 values as strings, uint32 and percentages as numbers) uploaded as a GitHub Actions
artifact named `<version>_report`. No ROM is needed:
- the denominator is the fixed function catalogue in `docs/progress-catalogues/`;
- the numerator is every function in `config/matches.json`.

Each matches.json function is matching C. The repository only receives accepted checkpoints:
two byte-identical full-ROM builds, the full test suite and independent source review.

Standard library only, so CI needs nothing but Python 3.
"""
from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
CATALOGUES = ROOT / "docs/progress-catalogues"
CATEGORY_NAMES = {"main_14400": "Main game", "resident": "Resident (boot + libultra)"}


def measures(functions: list[dict], matched: set[tuple[str, str]], units: int, complete_units: int) -> dict:
    total = sum(f["size"] for f in functions)
    done = [f for f in functions if (f["image_id"], f["symbol"]) in matched]
    done_bytes = sum(f["size"] for f in done)
    percent = 100.0 * done_bytes / total if total else 0.0
    function_percent = 100.0 * len(done) / len(functions) if functions else 0.0
    return {
        "fuzzy_match_percent": percent,
        "total_code": str(total),
        "matched_code": str(done_bytes),
        "matched_code_percent": percent,
        "total_functions": len(functions),
        "matched_functions": len(done),
        "matched_functions_percent": function_percent,
        "complete_code": str(done_bytes),
        "complete_code_percent": percent,
        "total_units": units,
        "complete_units": complete_units,
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--version", default="jp", help="version slug (artifact name is <version>_report)")
    parser.add_argument("--output", type=Path, default=Path("report.json"))
    parser.add_argument("--summary", type=Path, help="also write a Markdown summary (e.g. $GITHUB_STEP_SUMMARY)")
    args = parser.parse_args()

    catalogues = sorted(CATALOGUES.glob("*.json"))
    if len(catalogues) != 1:
        print(f"expected exactly one catalogue in {CATALOGUES}, found {len(catalogues)}", file=sys.stderr)
        return 1
    catalogue = json.loads(catalogues[0].read_text())
    functions = catalogue["identity"]["functions"]
    rows = json.loads((ROOT / "config/matches.json").read_text())["functions"]
    matched = {(row.get("image_id", "resident"), row["symbol"]) for row in rows}
    known = {(f["image_id"], f["symbol"]) for f in functions}
    unknown = sorted(matched - known)
    if unknown:
        print(f"matches.json names functions outside the catalogue: {unknown[:10]}", file=sys.stderr)
        return 1

    images = sorted({f["image_id"] for f in functions})
    units, categories = [], []
    for image in images:
        image_functions = sorted((f for f in functions if f["image_id"] == image), key=lambda f: f["vram_start"])
        image_measures = measures(image_functions, matched, 1, 0)
        complete = image_measures["matched_functions"] == image_measures["total_functions"]
        image_measures["complete_units"] = int(complete)
        units.append({
            "name": image,
            "measures": image_measures,
            "functions": [{
                "name": f["symbol"],
                "size": str(f["size"]),
                "fuzzy_match_percent": 100.0 if (image, f["symbol"]) in matched else 0.0,
                "metadata": {"virtual_address": str(f["vram_start"])},
            } for f in image_functions],
            "metadata": {"complete": complete, "progress_categories": [image]},
        })
        categories.append({"id": image, "name": CATEGORY_NAMES.get(image, image),
                           "measures": measures(image_functions, matched, 1, int(complete))})
    report = {
        "measures": measures(functions, matched, len(units), sum(u["metadata"]["complete"] for u in units)),
        "units": units,
        "version": 1,
        "categories": categories,
    }
    args.output.write_text(json.dumps(report, indent=1) + "\n")

    lines = [f"## Shiren 2 ({args.version}) matching-C progress", "",
             "| Image | Matched functions | Matched code bytes | Code % |", "| --- | ---: | ---: | ---: |"]
    for scope, m in [(c["name"], c["measures"]) for c in categories] + [("**Total**", report["measures"])]:
        lines.append(f"| {scope} | {m['matched_functions']:,} / {m['total_functions']:,} | "
                     f"{int(m['matched_code']):,} / {int(m['total_code']):,} | {m['matched_code_percent']:.4f}% |")
    lines += ["", "Denominator: provisional mapped CPU catalogue (main + resident images). "
              "Overlays and the complete-game denominator are not yet counted."]
    text = "\n".join(lines) + "\n"
    print(text)
    if args.summary:
        with args.summary.open("a") as handle:
            handle.write(text)
    return 0


if __name__ == "__main__":
    sys.exit(main())
