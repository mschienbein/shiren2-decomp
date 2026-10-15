"""Map-level vtable rows once the witness split is wired into generate (opus-02, for split_rows)."""
from __future__ import annotations

import copy
import hashlib
import json
import os
import sys
import unittest
from pathlib import Path

PROJECT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(PROJECT / "tools"))
import gxxvtable  # noqa: E402
import rommap  # noqa: E402
from images import load_inventory  # noqa: E402

PRIVATE_ROM = Path(os.environ.get("H21_TEST_ROM", str(PROJECT.parent / "inputs/r1-build/shiren2.z64")))


@unittest.skipUnless(PRIVATE_ROM.is_file(), "Private copied original required")
class SplitVtableRowTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls) -> None:
        cls.original = PRIVATE_ROM.read_bytes()
        cls.inventory = load_inventory(PROJECT / "config/images.json")
        cls.rules_bytes = (PROJECT / rommap.RULES_PATH).read_bytes()
        cls.rules = json.loads(cls.rules_bytes)
        cls.document = json.loads((PROJECT / rommap.MAP_PATH).read_text())
        cls.rom_map = rommap.load(PROJECT, cls.original)
        cls.evidence = rommap.load_evidence(cls.original, cls.rules)[gxxvtable.INPUT]

    def validate(self, document: dict) -> rommap.RomMap:
        return rommap.validate(document, self.original, self.inventory, self.rules,
                               rules_sha256=hashlib.sha256(self.rules_bytes).hexdigest())

    def rehash(self, ranges: list) -> dict:
        """Recompute the identities of edited rows, keeping the map's split ledger."""
        return rommap._document(self.original, ranges, rules_sha256=self.document["rules_sha256"],
                                catalogue_id=self.document["cpu_catalogue_id"], catalogue=self.document["cpu_catalogue"],
                                splits=self.document["splits"])

    def test_vtable_rows_are_exactly_the_pinned_witnesses(self) -> None:
        rows = {(s, e): ref for s, e, _, ref in self.rom_map.of_class(gxxvtable.CLASS)}
        self.assertEqual(rows, {(r["start"], r["end"]): r["id"] for r in self.evidence["vtables"]})
        self.assertEqual(sum(e - s for s, e in rows), 38168)
        padding = sum(e - s for s, e, *_ in self.rom_map.of_class("cpu_data_padding"))  # cpu-data-padding layer
        self.assertEqual(sum(e - s for s, e, *_ in self.rom_map.of_class("cpu_data", gxxvtable.CLASS)) + padding, 177984)
        for start in (0x11CF20, 0x11CF70):  # no proven install at generation: stays open
            self.assertEqual(next(r for r in self.rom_map.ranges if r[0] <= start < r[1])[2], "cpu_data")
        report = rommap.account(self.rom_map, self.original, PROJECT, [], self.rules)
        self.assertEqual(report["status_bytes"]["cpu_data:classified_binary"], 38168)
        self.assertEqual(report["status_bytes"]["cpu_data:open"], 177984 - 38168 - padding)
        self.assertNotIn("cpu_data:owned_c", report["status_bytes"])
        self.assertEqual({r["status"] for r in report["range_statuses"] if r["class"] == gxxvtable.CLASS},
                         {"classified_binary"})

    def test_rehashed_forged_split_rows_fail(self) -> None:
        ranges = self.document["ranges"]
        index = next(i for i, r in enumerate(ranges) if r[2] == gxxvtable.CLASS and ranges[i + 1][2] == "cpu_data"
                     and not self.original[ranges[i + 1][0]:ranges[i + 1][1]].strip(b"\0"))
        fill = ranges[index + 1]
        cases = {
            "zero fill relabelled": lambda rows: rows[index + 1].__setitem__(
                slice(2, 4), [gxxvtable.CLASS, rommap._vtable_ref(self.inventory, fill[0])]),
            "vtable relabelled cpu_data": lambda rows: rows[index].__setitem__(slice(2, 4), ["cpu_data", "main_14400"]),
            "vtable extended over its fill": lambda rows: (rows[index].__setitem__(1, fill[1]), rows.pop(index + 1)),
            "vtable merged back": lambda rows: (rows[index].__setitem__(slice(1, 4), [fill[1], "cpu_data", "main_14400"]),
                                                rows.pop(index + 1)),
        }
        for name, edit in cases.items():
            rows = copy.deepcopy(ranges)
            edit(rows)
            with self.subTest(name=name), self.assertRaises(rommap.MapError):
                self.validate(self.rehash(rows))
        self.assertEqual(self.validate(self.document).map_id, self.document["map_id"])
        self.assertEqual(self.validate(self.rehash(copy.deepcopy(ranges))).map_id, self.document["map_id"])


if __name__ == "__main__":
    unittest.main(verbosity=2)
