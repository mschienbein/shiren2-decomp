"""Asset format witnesses: positive typing, consumer cross-checks and tamper resistance."""
from __future__ import annotations

import copy
import hashlib
import json
import os
import struct
import sys
import unittest
from unittest import mock
from pathlib import Path

PROJECT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(PROJECT / "tools"))
import assetformat
import mipsflow
import rommap
from assetformat import PAD, AssetFormatError, Span

PRIVATE_ROM = Path(os.environ.get("H21_TEST_ROM", str(PROJECT.parent / "inputs/r1-build/shiren2.z64")))
PTR1 = (0x8CA860, 0x8D4290)
WAVE1 = (0x8D4290, 0xAEAE00)
PTR2 = (0xAEAE00, 0xAF67E0)
WAVE2 = (0xAF67E0, 0xD6ED70)
TOOL_SLOT, TOOL_LENGTH = 0x25A530, 8685  # wave 266 of bank 2
T4_TABLE, T4_RECORDS = 0x10F840, 25      # main_14400 D_8013C800: {gfxStart, gfxEnd, dataStart, dataEnd}


def t4_windows(original: bytes, pair: int) -> list[tuple[int, int]]:
    return [struct.unpack_from(">II", original, T4_TABLE + 16 * k + pair) for k in range(T4_RECORDS)]


_OWNER_EVIDENCE: dict = {}


def owner_words(original: bytes, rules: dict) -> dict:
    """rommap.owner_words for `original`. A full-document replay needs it once a witness uses owner_word
    anchors; the pinned evidence is loaded once from the fixed original."""
    if "evidence" not in _OWNER_EVIDENCE:
        _OWNER_EVIDENCE["evidence"] = rommap.load_evidence(PRIVATE_ROM.read_bytes(), rules)
    return rommap.owner_words(original, rules, _OWNER_EVIDENCE["evidence"])


_BASE_ROWS: dict = {}


def map_base_rows() -> dict:
    """{interval: (class, ref)} of the stored map before witness sub-range splits (replay's base_rows),
    read once from the generated map."""
    if "rows" not in _BASE_ROWS:
        document = json.loads((PROJECT / rommap.MAP_PATH).read_text())
        _BASE_ROWS["rows"] = {(r[0], r[1]): (r[2], r[3])
                              for r in rommap.unsplit_rows(document["ranges"], document["splits"])}
    return _BASE_ROWS["rows"]


def lw(rt: int, offset: int, base: int) -> int:
    return (0x23 << 26) | (base << 21) | (rt << 16) | offset


class TilingTests(unittest.TestCase):
    data = bytes(range(1, 33)) + bytes(16)

    def test_exact_typed_tiling_passes_and_accepts_plain_tuples(self) -> None:
        spans = [(0, 4, "rec.a", 0), Span(4, 32, "rec.b", 0), (32, 36, PAD, 32)]
        ordered = assetformat.check_tiling(self.data, spans, [(0, 36)])
        self.assertEqual([s.field for s in ordered], ["rec.a", "rec.b", PAD])
        self.assertEqual(assetformat.span_digest(ordered), assetformat.span_digest(sorted(Span(*s) for s in spans)))

    def test_gaps_overlaps_escapes_and_bad_pads_fail(self) -> None:
        cases = {
            "gap": [(0, 4, "rec.a", 0), (8, 40, "rec.b", 0)],
            "overlap": [(0, 8, "rec.a", 0), (4, 40, "rec.b", 0)],
            "short": [(0, 32, "rec.a", 0)],
            "escape": [(0, 40, "rec.a", 0), (40, 44, "rec.b", 40)],
            "nonzero pad": [(0, 4, PAD, 0), (4, 40, "rec.a", 4)],
            "long pad": [(0, 8, "rec.a", 0), (8, 24, PAD, 8), (24, 40, "rec.b", 0)],
            "unqualified field": [(0, 40, "blob", 0)],
            "base after field": [(0, 40, "rec.a", 4)],
            "empty": [(0, 0, "rec.a", 0), (0, 40, "rec.b", 0)],
        }
        for name, spans in cases.items():
            with self.subTest(name), self.assertRaises(AssetFormatError):
                assetformat.check_tiling(self.data, spans, [(0, 40)])

    def test_zero_or_pad_only_ranges_never_qualify(self) -> None:
        zeros = bytes(64)
        with self.assertRaises(AssetFormatError):
            assetformat.check_tiling(zeros, [(0, 8, PAD, 0), (8, 16, PAD, 8)], [(0, 16)])
        with self.assertRaises(AssetFormatError):  # pads above 1/8 of the range
            assetformat.check_tiling(zeros, [(0, 2, "rec.a", 0), (2, 16, PAD, 2)], [(0, 16)])

    def test_zero_dominated_ranges_fail_unless_the_format_requires_the_zeros(self) -> None:
        zeros = bytes(64)
        cases = {
            "fully typed all-zero range": (zeros, [(0, 4, "rec.a", 0), (4, 16, "rec.b", 0)]),
            "one nonzero byte in sixteen": (bytes(15) + b"\x01", [(0, 16, "rec.a", 0)]),
            "three nonzero bytes in sixteen": (bytes(13) + b"\x01" * 3, [(0, 16, "rec.a", 0)]),
            "zero data under a required-zero name": (b"\x01" + bytes(15), [(0, 4, "bank.flags", 0), (4, 16, "rec.b", 0)]),
        }
        for name, (data, spans) in cases.items():
            with self.subTest(name), self.assertRaisesRegex(AssetFormatError, "dominated by zero|Required-zero"):
                assetformat.check_tiling(data, spans, [(0, 16)])
        # A quarter of nonzero bytes, or zeros the format itself requires, qualify.
        assetformat.check_tiling(bytes(12) + b"\x01" * 4, [(0, 16, "rec.a", 0)], [(0, 16)])
        assetformat.check_tiling(zeros, [(0, 4, "bank.flags", 0), (4, 16, "rec.b", 0)], [(0, 16)])

    def test_many_ranges_tile_and_crossing_or_stray_spans_fail(self) -> None:
        data = bytes(range(1, 65))
        ranges = [(0, 8), (16, 24), (32, 40)]
        spans = [(a + k, a + k + 4, "rec.x", a) for a, _ in ranges for k in (0, 4)]
        self.assertEqual(len(assetformat.check_tiling(data, spans, ranges)), 6)
        cases = {
            "span crossing into a range": [(12, 20, "rec.x", 12), (20, 24, "rec.y", 12)],
            "span crossing out of a range": [(16, 20, "rec.x", 16), (20, 28, "rec.y", 16)],
            "span between ranges": [(16, 24, "rec.x", 16), (24, 28, "rec.y", 24)],
            "duplicate span": [(16, 20, "rec.x", 16), (16, 20, "rec.x", 16), (20, 24, "rec.y", 16)],
            "overlapping spans": [(16, 22, "rec.x", 16), (20, 24, "rec.y", 16)],
        }
        for name, middle in cases.items():
            mixed = [s for s in spans if not 16 <= s[0] < 24] + middle
            with self.subTest(name), self.assertRaises(AssetFormatError):
                assetformat.check_tiling(data, mixed, ranges)
        # Thousands of ranges in one witness tile in linear time.
        big = bytes(range(1, 256)) * 160
        many = [(4 * i, 4 * i + 4) for i in range(10000)]
        self.assertEqual(len(assetformat.check_tiling(big, [(a, b, "rec.x", a) for a, b in many], many)), 10000)


class ConsumerReadTests(unittest.TestCase):
    def setUp(self) -> None:
        code = struct.pack(">II", lw(2, 4, 4), (0x24 << 26) | (4 << 21) | (3 << 16) | 8)  # lw v0,4(a0); lbu v1,8(a0)
        self.data = code + bytes(range(1, 25))
        self.function = {"symbol": "func_0", "rom_start": 0, "size": 8,
                         "sha256": hashlib.sha256(code).hexdigest()}
        self.catalogue = {("func_0", 0, 8): self.function["sha256"]}
        self.spans = [Span(8, 12, "rec.a", 8), Span(12, 16, "rec.b", 8), Span(16, 17, "rec.c", 8),
                      Span(17, 20, "rec.d", 8), Span(20, 24, "rec.a", 20), Span(24, 28, "rec.b", 20),
                      Span(28, 29, "rec.c", 20), Span(29, 32, "rec.d", 20)]

    def consumer(self, *reads: dict) -> dict:
        return {**self.function, "image_id": "main_14400", "source": "src/x.c", "reads": list(reads)}

    def read(self, site: int, offset: int, width: int | None, struct_name: str = "rec") -> dict:
        word = struct.unpack_from(">I", self.data, site)[0]
        return {"site": site, "word": f"{word:08X}", "struct": struct_name, "offset": offset, "width": width}

    def test_accesses_matching_every_instance_pass(self) -> None:
        matched = assetformat.check_reads(self.data, [self.consumer(self.read(0, 4, 4), self.read(4, 8, 1))],
                                          self.spans, self.catalogue)
        self.assertEqual(matched, {(0, "rec"), (4, "rec")})
        # An access whose struct has no instance here is left to the per-format coverage check.
        self.assertEqual(assetformat.check_reads(self.data, [self.consumer(self.read(0, 4, 4, "other"))],
                                                 self.spans, self.catalogue), set())

    def test_wrong_offset_width_word_struct_or_function_fails(self) -> None:
        bad = {
            "offset": self.consumer({**self.read(0, 4, 4), "offset": 8}),
            "width": self.consumer(self.read(4, 8, 4)),
            "word": self.consumer({**self.read(0, 4, 4), "word": "8C820008"}),
            "site outside": self.consumer({**self.read(0, 4, 4), "site": 8}),
            "no reads": self.consumer(),
        }
        for name, consumer in bad.items():
            with self.subTest(name), self.assertRaises(AssetFormatError):
                assetformat.check_reads(self.data, [consumer], self.spans, self.catalogue)
        with self.assertRaises(AssetFormatError):
            assetformat.check_reads(self.data, [self.consumer(self.read(0, 4, 4))], self.spans,
                                    {("func_0", 0, 8): "0" * 64})

    def test_one_instance_without_the_field_fails(self) -> None:
        spans = [s for s in self.spans if (s.start, s.field) != (24, "rec.b")] + [Span(24, 28, "other.x", 24)]
        with self.assertRaises(AssetFormatError):
            assetformat.check_reads(self.data, [self.consumer(self.read(0, 4, 4))], spans, self.catalogue)


class AnchorTests(unittest.TestCase):
    table = {"T": {"id": "T", "rom_start": 16, "rom_end": 48, "record_size": 16, "pair_offsets": [8]}}
    data = bytes(24) + struct.pack(">II", 0x100, 0x180) + bytes(8) + struct.pack(">II", 0x180, 0x200) + bytes(8)

    def value(self, **anchor) -> int:
        record = {"kind": "table_word", "target": "range", "index": 0, **anchor}
        return assetformat._anchor_value(self.data, record, {}, lambda *_: 0, self.table, "anchor")

    def test_table_words_are_owner_pair_start_or_end_words(self) -> None:
        self.assertEqual(self.value(site=24, table="T", edge="start"), 0x100)
        self.assertEqual(self.value(site=28, table="T", edge="end"), 0x180)
        self.assertEqual(self.value(site=40, table="T", edge="start"), 0x180)
        for site, edge, table in ((28, "start", "T"), (24, "end", "T"), (32, "start", "T"), (8, "start", "T"),
                                  (24, "start", "U")):
            with self.subTest(site=site, edge=edge, table=table), self.assertRaises(AssetFormatError):
                self.value(site=site, table=table, edge=edge)
        with self.assertRaises(AssetFormatError):
            assetformat._anchor_value(self.data, {"kind": "guess", "site": 24}, {}, lambda *_: 0, self.table, "a")

    def test_owner_words_resolve_their_window_edges(self) -> None:
        words = {"R": {24: (0x100, 0x120)}}

        def value(owner=words, **anchor) -> int:
            record = {"kind": "owner_word", "site": 24, "rule": "R", "target": "range", "index": 0,
                      "edge": "start", **anchor}
            return assetformat._anchor_value(self.data, record, {}, lambda *_: 0, self.table, "anchor", owner)

        self.assertEqual(value(), 0x100)
        self.assertEqual(value(edge="end"), 0x120)
        for name, change in {"unknown rule": {"rule": "S"}, "word not in rule": {"site": 28},
                             "rule not a name": {"rule": ["R"]}, "bad edge": {"edge": "middle"},
                             "extra member": {"table": "T"}}.items():
            with self.subTest(name), self.assertRaises(AssetFormatError):
                value(**change)
        for owner in (None, {}):
            with self.subTest(owner=owner), self.assertRaises(AssetFormatError):
                value(owner=owner)


class StatusTests(unittest.TestCase):
    def test_witness_closes_only_located_classes(self) -> None:
        self.assertEqual(rommap.research_status("asset_audio_wavetable"), "located")
        self.assertEqual(rommap.research_status("asset_audio_wavetable", True), "classified_binary")
        for cls in ("asset_hvqm2_video", "asset_raw_referenced"):
            self.assertEqual(rommap.research_status(cls), "located")
            self.assertEqual(rommap.research_status(cls, True), "classified_binary")
        for cls in ("cpu_data", "asset_raw_unattributed", "asset_yay0", "pad_align_zero", "cpu_text_function"):
            with self.subTest(cls), self.assertRaises(rommap.MapError):
                rommap.research_status(cls, True)


@unittest.skipUnless(PRIVATE_ROM.is_file(), "Private copied original required")
class LibmusWitnessTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls) -> None:
        cls.original = PRIVATE_ROM.read_bytes()
        cls.rules = json.loads((PROJECT / rommap.RULES_PATH).read_text())
        cls.evidence = rommap.load_evidence(cls.original, cls.rules)
        cls.document = cls.evidence[assetformat.INPUT]
        cls.map_rows = json.loads((PROJECT / rommap.MAP_PATH).read_text())["ranges"]
        cls.rows = {(r[0], r[1]): (r[2], r[3]) for r in cls.map_rows}
        cls.functions = json.loads((PROJECT / rommap.CATALOGUE_PATH).read_text())["identity"]["functions"]
        cls.parse = staticmethod(assetformat.PARSERS["libmus_bank"])

    def replay(self, document: dict, original: bytes | None = None) -> dict:
        return assetformat.replay(original or self.original, document, self.functions, self.rows,
                                  rommap.immediate_at, rommap.EXPECTED_SHA256,
                                  {row["id"]: row for row in self.rules["tables"]},
                                  owner_words(original or self.original, self.rules), map_base_rows())

    def mutated(self, offset: int, value: bytes) -> bytes:
        data = bytearray(self.original)
        data[offset:offset + len(value)] = value
        return bytes(data)

    def test_replay_closes_all_four_bank_rows(self) -> None:
        closed = self.replay(self.document)
        self.assertEqual({k: v for k, v in closed.items() if v.startswith("W_libmus")},
                         {PTR1: "W_libmus_bank1", WAVE1: "W_libmus_bank1",
                          PTR2: "W_libmus_bank2", WAVE2: "W_libmus_bank2"})
        self.assertEqual(sum(b - a for a, b in closed if closed[(a, b)].startswith("W_libmus")),
                         39472 + 2190192 + 47584 + 2590096)

    def test_rommap_load_and_account_carry_the_witnessed_rows(self) -> None:
        rom_map = rommap.load(PROJECT, self.original)
        t4 = t4_windows(self.original, 0) + t4_windows(self.original, 8)
        self.assertTrue(frozenset({PTR1, WAVE1, PTR2, WAVE2, *t4}) <= rom_map.format_witnessed)
        result = rommap.account(rom_map, self.original, PROJECT, [], self.rules)
        statuses = {(r["start"], r["end"]): r["status"] for r in result["range_statuses"]}
        for interval in (PTR1, WAVE1, PTR2, WAVE2, *t4):
            self.assertEqual(statuses[interval], "classified_binary")
        self.assertEqual(result["research_classified_bytes"] + result["research_open_bytes"], rommap.EXPECTED_SIZE)

    def test_wave_266_is_tool_record_debris_not_adpcm(self) -> None:
        # Wave 266 (+0x25A530, 8,685 bytes) uses predictors 4..15 of a 4-predictor book.
        with self.assertRaisesRegex(AssetFormatError, "predictor"):
            self.parse(self.original, (PTR2, WAVE2), {"context": ()})
        slot = WAVE2[0] + TOOL_SLOT
        self.assertEqual(self.original[slot + 1:slot + 5], b"SAMP")
        spans = sorted(assetformat._tool_records(self.original, slot, slot + TOOL_LENGTH))
        self.assertEqual(assetformat.check_tiling(self.original, spans, [(slot, slot + TOOL_LENGTH)]), spans)
        self.assertEqual([s.field for s in spans[-2:]],
                         ["toolrec.data_frames_truncated", "toolrec.data_partial_frame_truncated"])
        self.assertEqual(spans[-1].end, slot + TOOL_LENGTH)
        self.assertEqual(sum(s.field == "toolrec.samp_tag" for s in spans), 6)
        self.assertEqual([s.field for s in spans if s.field.startswith("toolrec.data_frames")],
                         ["toolrec.data_frames", "toolrec.data_frames_truncated"])

    def test_tool_record_tamper_and_misuse_fail(self) -> None:
        slot = WAVE2[0] + TOOL_SLOT
        records = [slot + r for r in (0x1, 0xD8, 0x1E2, 0x12E6, 0x13BF, 0x1496)]
        cases = {
            "second SAMP tag": (records[1] + 3, b"Q"),
            "INFO adpcm bytes": (records[0] + 8 + 30 + 12, b"\xda"),
            "NAME terminator": (records[0] + 8 + 8 + 21, b"X"),
            "LOOP end": (records[1] + 8 + 30 + 24 + 144 + 12, b"\xff\xff\x00\x00"),
            "odd pad": (records[0] + 0xD6, b"\x01"),
            "whole-record frame": (records[2] + 0xD8, b"\x0f"),
            "truncated frame": (records[5] + 0xD6 + 9 * 100, b"\x0f"),
            "CDBK order": (records[0] + 8 + 30 + 24 + 8, b"\x03"),
        }
        params = {"context": (), "tool_record_samples": [TOOL_SLOT]}
        self.parse(self.original, (PTR2, WAVE2), params)
        for name, (offset, value) in cases.items():
            with self.subTest(name), self.assertRaises(AssetFormatError):
                self.parse(self.mutated(offset, value), (PTR2, WAVE2), params)
        for name, bad in {"valid ADPCM slot": {"context": (), "tool_record_samples": [TOOL_SLOT, 0x10]},
                          "not an extent": {"context": (), "tool_record_samples": [TOOL_SLOT + 16]},
                          "context only": {"context": (WAVE2,), "tool_record_samples": [TOOL_SLOT]},
                          "duplicate": {"context": (), "tool_record_samples": [TOOL_SLOT, TOOL_SLOT]}}.items():
            with self.subTest(name), self.assertRaises(AssetFormatError):
                ranges = (PTR2,) if bad["context"] else (PTR2, WAVE2)
                self.parse(self.original, ranges, bad)

    def test_count_offset_length_and_frame_tamper_fail(self) -> None:
        p0, w0 = PTR1[0], WAVE1[0]
        table = struct.unpack_from(">I", self.original, p0 + 0x2C)[0]
        first = p0 + struct.unpack_from(">I", self.original, p0 + table)[0]
        base, length = struct.unpack_from(">II", self.original, first)
        cases = {
            "count": (p0 + 0x23, bytes([self.original[p0 + 0x23] + 1])),
            "wave slot": (p0 + table + 3, bytes([self.original[p0 + table + 3] + 4])),
            "wave length": (first + 4, struct.pack(">I", length + 16 * 9)),
            "wave base": (first, struct.pack(">I", base + 16)),
            "book pointer": (first + 16, b"\0\0\0\0"),
            "frame predictor": (w0 + base, bytes([self.original[w0 + base] | 0x0F])),
            "relocated flag": (p0 + 0x10, b"\x80\0\0\0"),
            "magic": (w0, b"N64 WaveTable5 \0"),
        }
        for name, (offset, value) in cases.items():
            with self.subTest(name), self.assertRaises(AssetFormatError):
                self.parse(self.mutated(offset, value), (PTR1, WAVE1), {"context": ()})

    def test_shifted_or_swapped_intervals_fail(self) -> None:
        for ranges, context in (((PTR1[0], PTR1[1] - 16), WAVE1), (PTR1, (WAVE1[0], WAVE1[1] - 16)),
                                (PTR1, WAVE2), (PTR2, WAVE1)):
            with self.subTest(ranges=ranges, context=context), self.assertRaises(AssetFormatError):
                spans = self.parse(self.original, (ranges,), {"context": (context,)})
                assetformat.check_tiling(self.original, spans, [ranges])

    def test_record_tamper_fails_closed(self) -> None:
        def edit(change) -> dict:
            document = copy.deepcopy(self.document)
            change(document)
            return document
        reads = lambda d: d["formats"]["libmus_bank"]["consumers"][0]["reads"]  # noqa: E731
        cases = {
            "digest": lambda d: d["witnesses"][0].update(span_digest="0" * 64),
            "count": lambda d: d["witnesses"][1].update(span_count=d["witnesses"][1]["span_count"] - 1),
            "read offset": lambda d: reads(d)[0].update(offset=0x14),
            "read word": lambda d: reads(d)[0].update(word="8E020014"),
            "read width": lambda d: reads(d)[0].update(width=2),
            "read struct": lambda d: reads(d)[0].update(struct="loop"),
            "consumer hash": lambda d: d["formats"]["libmus_bank"]["consumers"][0].update(sha256="0" * 64),
            "no anchors": lambda d: d["witnesses"][0].update(anchors=d["witnesses"][0]["anchors"][1:2]),
            "anchor site": lambda d: d["witnesses"][0]["anchors"][0].update(site=0x24B90),
            "anchor kind": lambda d: d["witnesses"][0]["anchors"][0].update(kind="table_word", table="T2"),
            "tool samples dropped": lambda d: d["witnesses"][1].update(params={}),
            "tool sample moved": lambda d: d["witnesses"][1].update(params={"tool_record_samples": [TOOL_SLOT - 16]}),
            "wrong ref": lambda d: d["witnesses"][0]["ranges"][0].update(ref="I_audio_ptr2"),
            "wrong class": lambda d: d["witnesses"][0]["ranges"][1].update(**{"class": "asset_raw_referenced"}),
            "range hash": lambda d: d["witnesses"][0]["ranges"][0].update(sha256="0" * 64),
            "duplicate": lambda d: d["witnesses"].append(copy.deepcopy(d["witnesses"][1]) | {"id": "W_copy"}),
            "unknown format": lambda d: d.update(formats={"other": d["formats"]["libmus_bank"]}),
            "params": lambda d: d["witnesses"][0].update(params={"strict": False}),
            "target": lambda d: d.update(target_sha256="0" * 64),
            "unexercised access": lambda d: reads(d).append({**reads(d)[0], "struct": "absent_struct"}),
        }
        for name, change in cases.items():
            with self.subTest(name), self.assertRaises((AssetFormatError, rommap.MapError)):
                self.replay(edit(change))

    def test_changed_original_fails_before_parsing(self) -> None:
        with self.assertRaisesRegex(AssetFormatError, "content changed"):
            self.replay(self.document, self.mutated(PTR1[0] + 0x30, b"\x01"))


def _libmus_test_song(stream: bytes, keymap: bool, instruments: int) -> bytes:
    """A minimal one-channel song: header, tables, the instrument table, one envelope, optional
    keymap record 0 (instrument 1, envelope 0), a master stream (0x80) and the channel `stream`."""
    table = 0x44
    envelopes = table + 2 * instruments
    envelopes += (-envelopes) % 4
    keymaps = envelopes + 7
    master = keymaps + (6 if keymap else 0)
    words = [0x215, 1, instruments, 0x38, 0x3C, 0x40, envelopes, keymaps if keymap else envelopes, table, master,
             0, 0, 0, 0, master + 1, 0, 0]
    data = bytearray(struct.pack(">17I", *words))
    data += b"".join(struct.pack(">H", 5 + i) for i in range(instruments))
    data += bytes(envelopes - len(data)) + bytes([0x10, 0x7F, 0x20, 0x60, 0x30, 0x40, 0x50])
    if keymap:
        data += struct.pack(">HHBB", 1, 0, 0, 0x3C)
    data += b"\x80" + stream
    return bytes(data + bytes((-len(data)) % 16))


def _libmus_emulated_level_reads(data: bytes, s: int, frames: int, master_first: bool, marker: int | None = None) -> set:
    """Independent frame emulation of one song from the matched C (not the parser's model): the byte
    addresses its level updates read in `frames` frames after a func_80129EE0 start, or after a
    func_80129F10 start in which every populated voice seeks to its first 0xAB with id `marker`.
    Seek notes use func_80129F10's duration rule (an explicit duration clears readTicks), playback
    notes func_8012AF30's; the selected marker's duration is stored in +0x9A, and func_8012ACC0 skips
    func_8012AF30 while +0x9A is 0x7FFF. RuntimeError when a seek can never return."""
    u32 = lambda a: struct.unpack_from(">I", data, a)[0]  # noqa: E731
    operands = {0x81: -1, 0x82: 1, 0x84: 7, 0x85: 1, 0x86: 2, 0x87: 1, 0x88: 3, 0x89: 3, 0x8B: -1, 0x8D: 1,
                0x8F: 1, 0x90: -1, 0x95: 1, 0x97: 3, 0x9B: 1, 0x9C: 1, 0x9D: 2, 0x9E: -1, 0xA0: 1, 0xA1: 6,
                0xA2: 1, 0xA3: 2, 0xA4: 2, 0xA5: 2, 0xA6: 1, 0xA7: -1, 0xA8: 1, 0xA9: 1, 0xAA: 1, 0xAB: -2}

    def voice(cursor, levels):
        return {"pc": cursor, "time": 0, "next": 0, "step": 0x6000 // 60, "rv": 1, "ticks": 0, "rt": 0,
                "ticks9a": 1, "stack": [], "levels": [[at, 1, 0] for at in levels if at]}
    master = voice(s + u32(s + 0x24), ())
    channels = []
    for i in range(u32(s + 4)):
        c, v, p = (u32(s + u32(s + table) + 4 * i) for table in (0xC, 0x10, 0x14))
        if c:
            channels.append(voice(s + c, (s + v if v else 0, s + p if p else 0)))
    read = set()

    def command(vc, pc):                                  # one D_801487D0 handler; 0 = ended
        op, at = data[pc], pc + 1
        n = operands.get(op, 0)
        value = data[at] if n else None
        if n == -1:
            value, at = (((data[at] & 0x7F) << 8) | data[at + 1], at + 2) if data[at] & 0x80 else (data[at], at + 1)
        elif n == -2:
            at += 2 + (1 if data[at + 1] & 0x80 else 0)
        else:
            at += n
        if op == 0x80:
            vc["levels"] = []
            return 0
        if op == 0x85:
            for other in [master] + channels:
                other["step"] = (value * 0x6000 // 120) // 60
        vc.update({0x8B: {"ticks": value}, 0xAC: {"ticks": 0}, 0x8C: {"rt": 1}, 0x99: {"rv": 1},
                   0x9A: {"rv": 0}, 0x9B: {"rv": 0}}.get(op, {}))
        if op == 0x95:
            vc["stack"].append([at, [list(x[:2]) for x in vc["levels"]], data[pc + 1]])
        elif op == 0x96:
            top = vc["stack"][-1]
            if top[2] != 0xFF:
                top[2] = (top[2] - 1) & 0xFF
                if top[2] == 0:
                    vc["stack"].pop()
                    return at
            at = top[0]
            for level, saved in zip(vc["levels"], top[1]):
                level[0], level[1] = saved[0], saved[1]
        return at

    def duration(pc):
        return (pc + 2, (data[pc] & 0x7F) << 8 | data[pc + 1]) if data[pc] & 0x80 else (pc + 1, data[pc])

    def note(vc, pc, seeking):
        pc += 1
        if vc["rv"]:
            vc["rv"] = 0 if data[pc] & 0x80 else 1
            pc += 1
        if seeking:                                       # func_80129F10
            if vc["ticks"] and not vc["rt"]:
                ticks = vc["ticks"]
            else:
                vc["rt"] = 0
                pc, ticks = duration(pc)
        elif vc["ticks"]:                                 # func_8012AF30
            if vc["rt"]:
                vc["rt"] = 0
                pc, ticks = duration(pc)
            else:
                ticks = vc["ticks"]
        else:
            pc, ticks = duration(pc)
        vc["ticks9a"] = ticks
        return pc, ticks

    def update(vc, once):                                 # func_8012BA20 / func_8012BADC (do-while)
        for level in vc["levels"]:
            first = once
            while first or level[2] - vc["time"] < 0:
                first = False
                level[2] += 0x100
                level[1] -= 1
                if level[1] == 0:
                    at = level[0]
                    wait = 1
                    if data[at] & 0x80:
                        wait = data[at + 1] + 2
                        if data[at + 1] & 0x80:
                            wait = ((data[at + 1] & 0x7F) << 8) + data[at + 2] + 2
                            read.add(at + 2)
                        read.add(at + 1)
                    read.add(at)
                    level[0], level[1] = at + 1 + (data[at] >> 7) * (1 + (data[at + 1] >> 7)), wait

    if marker is not None:                                # func_80129F10
        for vc in [master] + channels:
            pc, states = vc["pc"], set()
            while pc:
                if data[pc] >= 0x80:
                    if data[pc] == 0xAB and data[pc + 1] == marker:
                        break
                    if data[pc] == 0x96:
                        key = (pc, tuple((t[0], t[2]) for t in vc["stack"]), vc["rv"], vc["ticks"], vc["rt"])
                        if key in states:
                            raise RuntimeError(f"func_80129F10 seek of the voice at {vc['pc']:#x} never returns")
                        states.add(key)
                    pc = command(vc, pc)
                else:
                    pc, ticks = note(vc, pc, True)
                    vc["time"] += ticks << 8
            vc["pc"], vc["next"] = pc, vc["time"]
            if pc:
                pc, ticks = duration(pc + 2)
                vc["pc"], vc["ticks9a"], vc["time"] = pc, ticks, vc["time"] - (ticks << 8)
                update(vc, True)
    order = [master] + channels if master_first else channels + [master]
    for _ in range(frames):
        for vc in order:
            if not vc["pc"]:
                continue
            vc["time"] += vc["step"]
            if vc["ticks9a"] != 0x7FFF:                   # func_8012ACC0
                while vc["pc"] and vc["next"] - vc["time"] < 0:
                    pc = vc["pc"]
                    while pc and data[pc] >= 0x80:
                        pc = command(vc, pc)
                    if not pc:
                        vc["pc"] = 0
                        break
                    pc, ticks = note(vc, pc, False)
                    vc["pc"], vc["next"] = pc, vc["next"] + (ticks << 8)
            if vc["pc"]:
                update(vc, False)
    return read


def _libmus_level_song(level: bytes, ticks: int) -> bytes:
    """A minimal one-channel song whose channel 0 plays one note of `ticks` ticks (envelope 0,
    instrument 0) and ends, with `level` as its volume stream: header, tables, the instrument table,
    one envelope, the level stream, the master stream (0x80) and the channel stream."""
    level_at = 0x48 + 7
    master = level_at + len(level)
    channel = master + 1
    words = [0x215, 1, 1, 0x38, 0x3C, 0x40, 0x48, 0x48, 0x44, master, 0, 0, 0, 0, channel, level_at, 0]
    data = bytearray(struct.pack(">17I", *words)) + struct.pack(">H", 5) + bytes(2)
    data += bytes([0x10, 0x7F, 0x20, 0x60, 0x30, 0x40, 0x50]) + level + b"\x80"
    data += bytes([0x90, 0x00, 0x3C, 0x7F, ticks, 0x80])
    return bytes(data + bytes((-len(data)) % 16))


def _libmus_review_song(channel: bytes) -> bytes:
    """SolReviewSongsWave2-r2 evidence section 3, common one-channel layout: header words
    [215,1,1,38,3c,40,48,48,44,51,0,0,0,0], channel table [0x52], volume table [0x4F], pitch table
    [0], instrument 00 05 + 2 zero bytes, envelope 10 7f 20 60 30 40 50, volume 81 00, master 80,
    then the channel bytes and zero fill to 16."""
    data = bytearray(struct.pack(">14I", 0x215, 1, 1, 0x38, 0x3C, 0x40, 0x48, 0x48, 0x44, 0x51, 0, 0, 0, 0))
    data += struct.pack(">3I", 0x52, 0x4F, 0) + bytes.fromhex("00050000") + bytes.fromhex("107f2060304050")
    data += bytes.fromhex("8100") + b"\x80" + channel
    return bytes(data + bytes((-len(data)) % 16))


def _libmus_level_bytes(data: bytes, s: int, e: int) -> set:
    u32 = lambda a: struct.unpack_from(">I", data, a)[0]  # noqa: E731
    n = u32(s + 4)
    offsets = {u32(s + 0xC + 4 * k) for k in range(7)} | \
        {u32(s + u32(s + table) + 4 * i) for table in (0xC, 0x10, 0x14) for i in range(n)}
    order = sorted(offsets - {0})
    levels = {u32(s + u32(s + table) + 4 * i) for table in (0x10, 0x14) for i in range(n)} - {0}
    out = set()
    for o in levels:
        out |= set(range(s + o, s + next((x for x in order if x > o), e - s)))
    return out


@unittest.skipUnless(PRIVATE_ROM.is_file(), "Private copied original required")
class LibmusSongWitnessTests(unittest.TestCase):
    """opus-08 WP3 r2 + wave 2 r3: libmus songs (T6/T6_alias) and the fx bank I_audio_e53b20."""
    SONG = (0xE14680, 0xE1A470)      # keymaps, volume streams, finite voices; T6[49]: func_80129F10 marker 0
    SONG0 = (0xD6ED70, 0xD72020)     # finite, 9-byte end fill
    PITCH = (0xE4E5C0, 0xE4EB60)     # pitch and volume streams
    ENDLESS = (0xDBA290, 0xDC0AC0)   # 15 endless voices: every level event read, no overrun
    SPILL = (0xDB5F50, 0xDBA290)     # endless voices reading on into the next level region
    SMALL = (0xE47840, 0xE47920)     # one endless voice, 224 bytes
    SEEK = (0xDF04C0, 0xDF0770)      # T6[36] (func_80052AD8): a marker-0 start over-reads 1 tick: not witnessed here
    LOOPING = (0xD72020, 0xD73370)   # a voice's level schedule overruns into the master stream: not witnessed
    FX = (0xE4F660, 0xE53B20)        # unexplained remainder byte 0x46: not witnessed
    FX2 = (0xE53B20, 0xE53DB0)

    @classmethod
    def setUpClass(cls) -> None:
        cls.original = PRIVATE_ROM.read_bytes()
        cls.rules = json.loads((PROJECT / rommap.RULES_PATH).read_text())
        cls.document = rommap.load_evidence(cls.original, cls.rules)[assetformat.INPUT]
        cls.rows = {(r[0], r[1]): (r[2], r[3]) for r in json.loads((PROJECT / rommap.MAP_PATH).read_text())["ranges"]}
        cls.functions = json.loads((PROJECT / rommap.CATALOGUE_PATH).read_text())["identity"]["functions"]

    def u32(self, at: int) -> int:
        return struct.unpack_from(">I", self.original, at)[0]

    def patched(self, at: int, value: int, width: int = 1) -> bytes:
        data = bytearray(self.original)
        data[at:at + width] = value.to_bytes(width, "big")
        return bytes(data)

    def typed(self, fmt: str, interval, original: bytes | None = None) -> list:
        data = original or self.original
        spans = assetformat.PARSERS[fmt](data, (interval,), {"context": ()})
        return assetformat.check_tiling(data, spans, [interval])

    def replay(self, document: dict, original: bytes | None = None) -> dict:
        return assetformat.replay(original or self.original, document, self.functions, self.rows,
                                  rommap.immediate_at, rommap.EXPECTED_SHA256,
                                  {row["id"]: row for row in self.rules["tables"]},
                                  owner_words(original or self.original, self.rules), map_base_rows())

    def first_stream(self, song: tuple, table: int) -> int:
        s = song[0]
        return next(s + self.u32(s + self.u32(s + table) + 4 * i) for i in range(self.u32(s + 4))
                    if self.u32(s + self.u32(s + table) + 4 * i))

    def regions(self, song: tuple) -> list:
        """Every region offset the song's header and tables define."""
        s, n = song[0], self.u32(song[0] + 4)
        return sorted({self.u32(s + 0xC + 4 * k) for k in range(7)} |
                      {self.u32(s + self.u32(s + table) + 4 * i) for table in (0xC, 0x10, 0x14) for i in range(n)} - {0})

    def test_witnessed_songs_and_fx_bank(self) -> None:
        closed = self.replay(self.document)
        songs = {k for k, (cls, ref) in self.rows.items() if ref == "T6"}
        plain = {w["id"] for w in self.document["witnesses"]
                 if w["format"] in ("libmus_song", "libmus_fxbank") and not w["params"]}
        mine = {k: v for k, v in closed.items() if v in plain}
        self.assertEqual(len(songs), 86)
        self.assertEqual(len(mine), 26)
        self.assertEqual(sum(b - a for a, b in mine), 160256)
        self.assertIn(self.FX2, mine)
        for song in (self.SONG, self.ENDLESS, self.SPILL, self.SMALL):
            self.assertIn(song, mine)
        self.assertNotIn(self.FX, closed)
        self.assertNotIn(self.LOOPING, mine)
        self.assertNotIn(self.SEEK, mine)
        self.assertEqual({v for k, v in mine.items() if k in songs}, {f"libmus_song_{a:07x}" for a, _ in set(mine) & songs})

    def test_unexplained_fx_fill_cannot_grant_credit(self) -> None:
        self.assertEqual(self.original[0xE538F9], 0x46)
        with self.assertRaises(AssetFormatError):
            self.typed("libmus_fxbank", self.FX)
        self.assertTrue(self.typed("libmus_fxbank", self.FX, self.patched(0xE538F9, 0)))

    def test_level_schedule_reads_every_event_and_no_other_byte(self) -> None:
        with self.assertRaisesRegex(AssetFormatError, "reads past its streams into master region"):
            self.typed("libmus_song", self.LOOPING)
        with self.assertRaisesRegex(AssetFormatError, "into master region .*func_80129F10 marker 0 start"):
            self.typed("libmus_song", self.SEEK)
        # One 48-tick note at the default step 0x6000 / 60 = 409: level ticks 1..48 run before 0x80.
        exact, longer, shorter = (_libmus_level_song(bytes(n), 48) for n in (48, 60, 40))
        self.assertTrue(self.typed("libmus_song", (0, len(exact)), exact))
        with self.assertRaisesRegex(AssetFormatError, "12 events of the level region 0x4f are never read"):
            self.typed("libmus_song", (0, len(longer)), longer)
        with self.assertRaisesRegex(AssetFormatError, "reads past its streams into master region"):
            self.typed("libmus_song", (0, len(shorter)), shorter)
        for song in (self.SONG, self.PITCH, self.ENDLESS, self.SPILL, self.SMALL):
            self.assertTrue(self.typed("libmus_song", song))

    def test_schedule_agrees_with_frame_emulation(self) -> None:
        master = self.SEEK[0] + self.u32(self.SEEK[0] + 0x24)
        cases = ((self.SMALL, None, 20000, set()), (self.SONG, 0, 8000, set()), (self.SEEK, None, 20000, set()),
                 (self.SEEK, 1, 20000, set()), (self.SEEK, 0, 20000, {master, master + 1, master + 2}),
                 (self.LOOPING, None, 6000, None))
        for song, marker, frames, outside in cases:
            level = _libmus_level_bytes(self.original, *song)
            if outside is None:
                head = song[0] + self.u32(song[0] + 0x24)
                outside = {head, head + 1, head + 2}
            for master_first in (True, False):
                read = _libmus_emulated_level_reads(self.original, song[0], frames, master_first, marker)
                with self.subTest(song=hex(song[0]), marker=marker, master_first=master_first):
                    self.assertTrue(level <= read)
                    self.assertEqual(read - level, outside)

    # SolReviewSongsWave2-r2 evidence section 3: its exact synthetic songs (level region +0x4F..+0x51,
    # master byte 80 at +0x51, channel bytes from +0x52).
    def test_f10_seek_clears_read_ticks(self) -> None:
        song = _libmus_review_song(bytes.fromhex("90008c607f01ab0000900090008b30607f0180"))       # 3.1
        self.assertEqual(len(song), 112)
        for master_first in (True, False):
            self.assertEqual(_libmus_emulated_level_reads(song, 0, 10, master_first), {0x4F, 0x50})
            self.assertEqual(_libmus_emulated_level_reads(song, 0, 10, master_first, 0), {0x4F, 0x50, 0x51, 0x52, 0x53})
        with self.assertRaisesRegex(AssetFormatError, r"differs from the linear decode \(func_80129F10 marker 0 start\)"):
            self.typed("libmus_song", (0, len(song)), song)

    def test_f10_selected_stop_marker_is_refused(self) -> None:
        song = _libmus_review_song(bytes.fromhex("9000607f01ab00ffff607f0180"))                   # 3.2
        self.assertEqual(len(song), 96)
        for master_first in (True, False):
            self.assertEqual(_libmus_emulated_level_reads(song, 0, 20600, master_first), {0x4F, 0x50})
            self.assertEqual(_libmus_emulated_level_reads(song, 0, 20600, master_first, 0),
                             {0x4F, 0x50, 0x51, 0x52, 0x53})
        with self.assertRaisesRegex(AssetFormatError, r"0x7FFF stop value .*func_80129F10 marker 0 start"):
            self.typed("libmus_song", (0, len(song)), song)
        # An unselected AB 00 FF FF (marker 0 selects the earlier AB 00 00) is only skipped operands.
        control = _libmus_review_song(bytes.fromhex("9000ab0000607f01ab00ffff607f01" + "9000" * 7 + "80"))
        self.assertTrue(self.typed("libmus_song", (0, len(control)), control))

    def test_f10_seek_must_return_for_level_free_voices(self) -> None:
        song = (struct.pack(">14I", 0x215, 2, 1, 0x38, 0x40, 0x48, 0x54, 0x54, 0x50, 0x5D, 0, 0, 0, 0)     # 3.3
                + struct.pack(">6I", 0x5E, 0x64, 0, 0x5B, 0, 0) + bytes.fromhex("00050000107f2060304050")
                + bytes.fromhex("810080") + bytes.fromhex("95ff607f0196") + bytes.fromhex("9000607f01ab0000607f0180"))
        self.assertEqual(len(song), 112)
        self.assertEqual(_libmus_emulated_level_reads(song, 0, 100, True), {0x5B, 0x5C})
        with self.assertRaisesRegex(RuntimeError, "never returns"):
            _libmus_emulated_level_reads(song, 0, 100, True, 0)
        with self.assertRaisesRegex(AssetFormatError, r"seek of the voice at 0x5e never reaches marker 0"):
            self.typed("libmus_song", (0, len(song)), song)

    def test_f10_seek_does_not_store_mapped_instruments(self) -> None:
        # 81 02 then a keymap note: func_8012AF30 stores keyMap[0].instrument = 1 in +0xAE, the seek
        # does not, so after marker 0 and 9F the rest reads instrument entry 2 of a 2-entry table.
        song = _libmus_test_song(bytes.fromhex("9000607f0181029e00007f01ab00009f607f0180"), True, 2)
        with self.assertRaisesRegex(AssetFormatError, r"instrument entries \[2\] past the table \(func_80129F10 marker 0"):
            self.typed("libmus_song", (0, len(song)), song)

    def test_keymap_instrument_persists_after_clear(self) -> None:
        mapped = bytes([0x9E, 0x00, 0x00, 0x7F, 0x01, 0x9F, 0x60, 0x7F, 0x01, 0x80])
        song = _libmus_test_song(mapped, True, 2)
        with self.assertRaisesRegex(AssetFormatError, "instrument table"):
            self.typed("libmus_song", (0, len(song)), song)
        song = _libmus_test_song(bytes([0x60, 0x7F, 0x01]) + mapped, True, 2)
        self.assertTrue(self.typed("libmus_song", (0, len(song)), song))

    def test_loop_depth_is_four_slots(self) -> None:
        for depth in (4, 5):
            stream = bytes([0x90, 0x00]) + bytes([0x95, 0x01]) * depth + bytes([0x60, 0x7F, 0x01]) \
                + bytes([0x96]) * depth + bytes([0x60, 0x7F, 0x01]) * 4 + bytes([0x80])
            song = _libmus_test_song(stream, False, 1)
            if depth == 4:
                self.assertTrue(self.typed("libmus_song", (0, len(song)), song))
            else:
                with self.subTest(depth=depth), self.assertRaisesRegex(AssetFormatError, "saved-state slot"):
                    self.typed("libmus_song", (0, len(song)), song)

    def test_header_table_and_stream_tamper_fail(self) -> None:
        s, p, f = self.SONG[0], self.PITCH[0], self.FX2[0]
        ch0 = self.first_stream(self.SONG, 0xC)
        vol = self.first_stream(self.SONG, 0x10)
        vol_end = min(s + o for o in self.regions(self.SONG) if s + o > vol)
        at = last = vol
        while at < vol_end:                     # the region's last event, then a value byte
            last = at                           # whose wait would lie past the region
            at += 1 + ((self.original[at] >> 7) and 1 + (self.original[at + 1] >> 7))
        truncated = bytearray(self.original)
        truncated[last:vol_end] = bytes(vol_end - last - 1) + b"\x81"
        cases = {
            "signature": ("libmus_song", self.SONG, self.patched(s + 3, 0x16)),
            "channel count": ("libmus_song", self.SONG, self.patched(s + 7, self.original[s + 7] + 1)),
            "instrument count": ("libmus_song", self.SONG, self.patched(s + 11, self.original[s + 11] + 1)),
            "relocated flag": ("libmus_song", self.SONG, self.patched(s + 0x2B, 1)),
            "reserved word": ("libmus_song", self.SONG, self.patched(s + 0x2F, 1)),
            "channel pointer": ("libmus_song", self.SONG, self.patched(s + 0x38, self.u32(s + 0x38) + 1, 4)),
            "unhandled opcode": ("libmus_song", self.SONG, self.patched(ch0, 0xAD)),
            "fx call in a song": ("libmus_song", self.SONG, self.patched(ch0, 0xA7)),
            "volume end": ("libmus_song", self.SONG, bytes(truncated)),
            "keymap pointer": ("libmus_song", self.SONG, self.patched(s + 0x1F, (self.u32(s + 0x1C) + 6) & 0xFF)),
            "envelope pointer": ("libmus_song", self.SONG, self.patched(s + 0x1B, (self.u32(s + 0x18) + 7) & 0xFF)),
            "end fill": ("libmus_song", self.SONG0, self.patched(self.SONG0[1] - 1, 1)),
            "effect count": ("libmus_fxbank", self.FX2, self.patched(f + 7, self.original[f + 7] - 1)),
            "fx instrument count": ("libmus_fxbank", self.FX2, self.patched(f + 11, self.original[f + 11] - 1)),
            "fx flags": ("libmus_fxbank", self.FX2, self.patched(f + 15, 1)),
            "fx entry offset": ("libmus_fxbank", self.FX2, self.patched(f + 0x23, self.original[f + 0x23] + 1)),
            "fx owner read": ("libmus_fxbank", self.FX2, self.patched(f + self.u32(f + 0x18), 0x90)),
        }
        for name, (fmt, interval, data) in cases.items():
            with self.subTest(name), self.assertRaises(AssetFormatError):
                self.typed(fmt, interval, data)
        for fmt, interval in (("libmus_song", self.SONG), ("libmus_song", self.SONG0), ("libmus_song", self.PITCH),
                              ("libmus_fxbank", self.FX2)):
            self.assertTrue(self.typed(fmt, interval))

    def test_shifted_or_grouped_intervals_fail(self) -> None:
        for fmt, ranges in (("libmus_song", ((self.SONG[0], self.SONG[1] - 16),)),
                            ("libmus_song", ((self.SONG[0], self.SONG[1] + 16),)),
                            ("libmus_song", (self.SONG, self.SONG0)),
                            ("libmus_fxbank", ((self.FX2[0], self.FX2[1] - 16),)),
                            ("libmus_song", (self.FX2,)), ("libmus_fxbank", (self.SONG,))):
            with self.subTest(fmt=fmt, ranges=ranges), self.assertRaises(AssetFormatError):
                spans = assetformat.PARSERS[fmt](self.original, ranges, {"context": ()})
                assetformat.check_tiling(self.original, spans, list(ranges))

    def test_record_tamper_fails_closed(self) -> None:
        def edit(change) -> dict:
            document = copy.deepcopy(self.document)
            change(document)
            return document
        song = lambda d: next(w for w in d["witnesses"] if w["format"] == "libmus_song")  # noqa: E731
        fxw = lambda d: next(w for w in d["witnesses"] if w["format"] == "libmus_fxbank")  # noqa: E731
        reads = lambda d: d["formats"]["libmus_song"]["consumers"][0]["reads"]  # noqa: E731
        cases = {
            "song digest": lambda d: song(d).update(span_digest="0" * 64),
            "fx count": lambda d: fxw(d).update(span_count=fxw(d)["span_count"] + 1),
            "anchor word": lambda d: song(d)["anchors"][0].update(site=song(d)["anchors"][0]["site"] + 8),
            "anchor table": lambda d: song(d)["anchors"][0].update(table="T2"),
            "no start anchor": lambda d: song(d).update(anchors=[a for a in song(d)["anchors"] if a["edge"] == "end"]),
            "read offset": lambda d: reads(d)[1].update(offset=8),
            "read struct": lambda d: reads(d)[1].update(struct="libmus_fxbank"),
            "params": lambda d: song(d).update(params={"strict": True}),
            "wrong ref": lambda d: fxw(d)["ranges"][0].update(ref="I_audio_e4f660"),
        }
        for name, change in cases.items():
            with self.subTest(name), self.assertRaises((AssetFormatError, rommap.MapError)):
                self.replay(edit(change))


class TransferReadTests(unittest.TestCase):
    """opus-05: whole-instance transfer reads (`addiu aN, zero, size` next to a pinned jal)."""

    def setUp(self) -> None:
        jal = (3 << 26) | 0x40000
        code = struct.pack(">III", jal, (0x09 << 26) | (6 << 16) | 8, 0)  # jal; addiu a2,zero,8; nop
        self.data = code + bytes(range(1, 17))
        self.function = {"symbol": "func_0", "rom_start": 0, "size": 12, "sha256": hashlib.sha256(code).hexdigest()}
        self.catalogue = {("func_0", 0, 12): self.function["sha256"]}
        self.spans = [Span(12, 16, "rec.a", 12), Span(16, 20, "rec.b", 12),
                      Span(20, 24, "rec.a", 20), Span(24, 28, "rec.b", 20)]

    def read(self, **change) -> dict:
        read = {"site": 4, "word": "24060008", "struct": "rec", "offset": 0, "width": 8,
                "transfer_call": {"site": 0, "word": "0C040000"}}
        read.update(change)
        return {**self.function, "image_id": "main_14400", "source": "src/x.c", "reads": [read]}

    def test_exact_instances_pass(self) -> None:
        self.assertEqual(assetformat.check_reads(self.data, [self.read()], self.spans, self.catalogue), {(4, "rec")})

    def test_wrong_size_offset_call_or_larger_instance_fail(self) -> None:
        cases = {
            "size": self.read(width=4),
            "offset": self.read(offset=4),
            "call word": self.read(transfer_call={"site": 0, "word": "0C040004"}),
            "call not adjacent": self.read(transfer_call={"site": 8, "word": "00000000"}),
            "with size_site": self.read(size_site=4, size_scale=1),
        }
        for name, consumer in cases.items():
            with self.subTest(name), self.assertRaises(AssetFormatError):
                assetformat.check_reads(self.data, [consumer], self.spans, self.catalogue)
        larger = self.spans + [Span(28, 32, "rec.c", 20)]
        with self.assertRaises(AssetFormatError):
            assetformat.check_reads(self.data + bytes(8), [self.read()], larger, self.catalogue)


class FrameReadTests(unittest.TestCase):
    """opus-05 r3: loads of a consumer's own stack copy of a whole-instance transfer ("frame")."""
    WORDS = [0x27BDFFE0,  # 0x00 addiu sp, sp, -0x20
             0x93A20011,  # 0x04 lbu v0, 0x11(sp)   (before the copy)
             0x24050008,  # 0x08 addiu a1, zero, 8 (size)
             0x0C040000,  # 0x0C jal
             0x27A60010,  # 0x10 addiu a2, sp, 0x10 (copy destination)
             0x93A20011,  # 0x14 lbu v0, 0x11(sp)   +1
             0x97A30016,  # 0x18 lhu v1, 0x16(sp)   +6/2
             0x93A20018,  # 0x1C lbu v0, 0x18(sp)   +8, past the copy
             0x27A70010,  # 0x20 addiu a3, sp, 0x10 (not next to the call)
             0xAFBF001C,  # 0x24 sw ra, 0x1C(sp)    (outside the copy)
             0x27BD0020]  # 0x28 addiu sp, sp, 0x20
    SPANS = [Span(44 + k, 44 + k + w, f"rec.{name}", 44) for k, w, name in ((0, 1, "a"), (1, 1, "b"), (2, 4, "c"), (6, 2, "d"))] + \
            [Span(52 + k, 52 + k + w, f"rec.{name}", 52) for k, w, name in ((0, 1, "a"), (1, 1, "b"), (2, 4, "c"), (6, 2, "d"))]

    def check(self, reads: list, words: dict | None = None) -> set:
        code = list(self.WORDS)
        for at, word in (words or {}).items():
            code[at // 4] = word
        packed = struct.pack(f">{len(code)}I", *code)
        function = {"symbol": "func_0", "rom_start": 0, "size": len(packed), "sha256": hashlib.sha256(packed).hexdigest()}
        consumer = {**function, "image_id": "main_14400", "source": "src/x.c", "reads": reads}
        return assetformat.check_reads(packed + bytes(range(1, 17)), [consumer], self.SPANS,
                                       {("func_0", 0, len(packed)): function["sha256"]})

    def transfer(self, struct_name: str = "rec") -> dict:
        return {"site": 8, "word": "24050008", "struct": struct_name, "offset": 0, "width": 8,
                "transfer_call": {"site": 12, "word": "0C040000"}}

    def load(self, site: int, offset: int, width: int, frame: int = 16, word: int | None = None,
             frame_word: int | None = None) -> dict:
        word = self.WORDS[site // 4] if word is None else word
        frame_word = self.WORDS[frame // 4] if frame_word is None else frame_word
        return {"site": site, "word": f"{word:08X}", "struct": "rec", "offset": offset, "width": width,
                "frame": {"site": frame, "word": f"{frame_word:08X}"}}

    def test_loads_of_the_stack_copy_pass(self) -> None:
        self.assertEqual(self.check([self.transfer(), self.load(0x14, 1, 1), self.load(0x18, 6, 2)]),
                         {(8, "rec"), (0x14, "rec"), (0x18, "rec")})

    def test_every_frame_rule_fails_closed(self) -> None:
        sb, lbu_a0 = 0xA3A20011, 0x90820011
        cases = {
            "store access": ([self.transfer(), self.load(0x14, 1, 1, word=sb)], {0x14: sb}, "not a later sp load"),
            "base not sp": ([self.transfer(), self.load(0x14, 1, 1, word=lbu_a0)], {0x14: lbu_a0}, "not a later sp load"),
            "immediate not disp + offset": ([self.transfer(), self.load(0x14, 2, 1)], None, "offset disagrees"),
            "frame not next to the call": ([self.transfer(), self.load(0x14, 1, 1, frame=0x20)], None,
                                           "not next to a pinned rec transfer"),
            "frame of another struct's transfer": ([self.transfer("other"), self.load(0x14, 1, 1)], None,
                                                   "not next to a pinned rec transfer"),
            # The load comes first so the frame rule, not the size's delay-slot liveness, rejects it.
            "frame is the size register": ([self.load(0x14, 1, 1, frame_word=0x27A50010), self.transfer()],
                                           {0x10: 0x27A50010}, "size argument"),
            "frame not an sp address": ([self.transfer(), self.load(0x14, 1, 1, frame=8)], None, "addiu aN, sp"),
            "read before the call": ([self.transfer(), self.load(0x04, 1, 1)], None, "not a later sp load"),
            "read past the transfer width": ([self.transfer(), self.load(0x1C, 8, 1)], None, "inside the 8-byte copy"),
            "sp store over the copy": ([self.transfer(), self.load(0x14, 1, 1)], {0x24: 0xA3A20012}, "overlaps the stack copy"),
            "extra sp write": ([self.transfer(), self.load(0x14, 1, 1)], {0x24: 0x0080E82D}, "writes sp"),
            "frame on a transfer read": ([{**self.transfer(), "frame": {"site": 16, "word": "27A60010"}}], None,
                                         "not a plain load"),
        }
        for name, (reads, words, message) in cases.items():
            with self.subTest(name), self.assertRaisesRegex(AssetFormatError, message):
                self.check(reads, words)

    def test_sol_r3_counterexamples_fail_closed(self) -> None:
        # SolReviewAssetStack r3 F1-F3, with the reviewer's exact instruction words in this fixture.
        cases = {
            "F1 balanced sp move between copy and load": (
                [self.transfer(), self.load(0x18, 6, 2)], {0x14: 0x27BDFFF0, 0x18: 0x97A30016, 0x1C: 0x27BD0010},
                "moves sp between the copy"),
            "F2 branch skips the copy": (
                [self.transfer(), self.load(0x14, 1, 1)], {0x04: 0x10000003}, "does not precede the load"),
            "F3 sc over the copy": (
                [self.transfer(), self.load(0x20, 1, 1, word=0x93A20011)],
                {0x14: 0xC3A20010, 0x18: 0x24020000, 0x1C: 0xE3A20010, 0x20: 0x93A20011}, "overlaps the stack copy"),
            "F3 scd over the copy": (
                [self.transfer(), self.load(0x20, 1, 1, word=0x93A20011)],
                {0x14: 0xD3A20010, 0x18: 0x24020000, 0x1C: 0xF3A2000C, 0x20: 0x93A20011}, "overlaps the stack copy"),
            "indirect jump hides the flow": (
                [self.transfer(), self.load(0x14, 1, 1)], {0x24: 0x00800008}, "unchanged on every path|indirect jumps"),
        }
        for name, (reads, words, message) in cases.items():
            with self.subTest(name), self.assertRaisesRegex(AssetFormatError, message):
                self.check(reads, words)
        # A branch that re-enters before the copy (a loop back to the size argument) keeps it valid.
        self.assertIn((0x14, "rec"), self.check([self.transfer(), self.load(0x14, 1, 1)], {0x24: 0x1000FFF8}))

    def test_sol_f2_counterexamples_fail_closed(self) -> None:
        # SolReviewAssetStack F2 P2-1..P2-3, with the reviewer's exact instruction words in this fixture.
        size_04 = {"site": 4, "word": "24050008", "struct": "rec", "offset": 0, "width": 8,
                   "transfer_call": {"site": 12, "word": "0C040000"}}
        cases = {
            "P2-1 delay slot rewrites the size (a1/a2 counterpart)": (
                [size_04, self.load(0x18, 6, 2, frame=8, frame_word=0x27A60010)],
                {0x04: 0x24050008, 0x08: 0x27A60010, 0x10: 0x24050004, 0x18: 0x97A30016}, "unchanged on every path"),
            "P2-2 delay slot rewrites the destination": (
                [size_04, self.load(0x14, 1, 1, frame=8, frame_word=0x27A60010)],
                {0x04: 0x24050008, 0x08: 0x27A60010, 0x10: 0x27A60018, 0x14: 0x93A20011}, "not a fresh destination"),
            "P2-2 re-copy after an sp move with the old destination": (
                [{"site": 0x0C, "word": "24050008", "struct": "rec", "offset": 0, "width": 8,
                  "transfer_call": {"site": 8, "word": "0C040000"}},
                 self.load(0x10, 1, 1, frame=4, frame_word=0x27A60010, word=0x93A20011)],
                {0x00: 0x27BDFFE0, 0x04: 0x27A60010, 0x08: 0x0C040000, 0x0C: 0x24050008, 0x10: 0x93A20011,
                 0x14: 0x27BDFFF0, 0x18: 0x1000FFFB, 0x1C: 0x00000000}, "not a fresh destination"),
            "P2-3 branch into the jr ra delay slot": (
                [self.transfer(), self.load(0x1C, 1, 1, word=0x93A20011)],
                {0x04: 0x10000004, 0x14: 0x03E00008, 0x18: 0x00000000, 0x1C: 0x93A20011},
                "does not precede the load"),
            "P2-3 branch into a branch-likely delay slot": (
                [self.transfer(), self.load(0x24, 1, 1, word=0x93A20011)],
                {0x04: 0x10000006, 0x1C: 0x5000FFFB, 0x20: 0x00000000, 0x24: 0x93A20011},
                # its BEQL also loops back to the JAL past the size, so the size's liveness fails first
                "unchanged on every path|does not precede the load"),
        }
        for name, (reads, words, message) in cases.items():
            with self.subTest(name), self.assertRaisesRegex(AssetFormatError, message):
                self.check(reads, words)
        # The engine runs each entered delay slot in both contexts: as its owner's slot and, from the
        # branch, as an ordinary instruction that falls through to the load without the copy.
        for words, entered in (({0x04: 0x10000004, 0x14: 0x03E00008, 0x18: 0}, 0x18),
                               ({0x04: 0x10000006, 0x1C: 0x5000FFFB, 0x20: 0}, 0x20)):
            code = list(self.WORDS)
            for at, word in words.items():
                code[at // 4] = word
            flow = assetformat._flow(struct.pack(f">{len(code)}I", *code), 0, 4 * len(code), None)
            self.assertEqual(flow.at(entered), [(entered, mipsflow.ORDINARY), (entered, mipsflow.SLOT)])
            self.assertFalse(flow.dominates(flow.ret(0x0C), (entered + 4, mipsflow.ORDINARY)))
        # A loop that moves sp and re-runs the size and the call, with the destination in the delay slot,
        # gives every copy a fresh destination of its own sp.
        self.assertEqual(self.check(
            [{"site": 4, "word": "24050008", "struct": "rec", "offset": 0, "width": 8,
              "transfer_call": {"site": 8, "word": "0C040000"}},
             self.load(0x10, 1, 1, frame=0x0C, frame_word=0x27A60010, word=0x93A20011)],
            {0x04: 0x24050008, 0x08: 0x0C040000, 0x0C: 0x27A60010, 0x10: 0x93A20011, 0x14: 0x27BDFFF0,
             0x18: 0x1000FFFA, 0x1C: 0x00000000}), {(4, "rec"), (0x10, "rec")})


class FetchCallTests(unittest.TestCase):
    """opus-05 script grammar: one-byte stream fetches through a pinned fetcher ("fetch_call"), with
    offsets derived from the entry, the base and the fetcher calls on every path."""
    FETCHER, HELPER, FETCHING, INDIRECT, CONSUMER = 0x00, 0x10, 0x18, 0x28, 0x38
    NOP, JR = 0x00000000, 0x03E00008

    @staticmethod
    def jal(rom: int) -> int:
        return 0x0C000000 | ((0x80000000 + rom) >> 2 & 0x3FFFFFF)

    SWITCHED, BAD_JR, DATA = 0x80, 0xAC, 0xC0
    # GCC switch dispatch on a0 with 2 cases and its table at `table` (ROM = VRAM - 0x80000000).
    @staticmethod
    def dispatch(default_skip: int, table: int) -> list[int]:
        return [0x2C820002, 0x10400000 | default_skip, 0x00000000, 0x00041080, 0x3C018000, 0x00220821,
                0x8C220000 | table, 0x00400008, 0x00000000]   # sltiu; beqz; nop; sll; lui; addu; lw; jr v0; nop

    def build(self, consumer: list[int], fetcher: list[int] | None = None) -> tuple[bytes, dict, object, list]:
        words = {"func_f": (self.FETCHER, fetcher or [0x90820000, self.JR, self.NOP, self.NOP]),   # lbu v0, 0(a0)
                 "func_h": (self.HELPER, [self.JR, self.NOP]),
                 "func_b": (self.FETCHING, [self.jal(self.FETCHER), self.NOP, self.JR, self.NOP]),
                 "func_x": (self.INDIRECT, [0x0040F809, self.NOP, self.JR, self.NOP]),               # jalr v0
                 "func_s": (self.SWITCHED, self.dispatch(7, 0xB8) + [self.JR, self.NOP]),             # fetch-free switch
                 "func_j": (self.BAD_JR, [0x00400008, self.NOP]),                                    # jr v0, no switch
                 "func_k": (self.CONSUMER, consumer)}
        data = bytearray(self.DATA)
        data[0xB8:0xC0] = struct.pack(">II", 0x800000A4, 0x800000A4)   # func_s cases: its jr ra
        rows = []
        for name, (start, body) in words.items():
            packed = struct.pack(f">{len(body)}I", *body)
            data[start:start + len(packed)] = packed
            rows.append({"symbol": name, "image_id": "main_14400", "rom_start": start, "size": len(packed),
                         "sha256": hashlib.sha256(packed).hexdigest(), "vram_start": 0x80000000 + start})
        data = bytes(data) + bytes(range(1, 9))
        catalogue = {(r["symbol"], r["rom_start"], r["size"]): r["sha256"] for r in rows}
        return data, catalogue, assetformat.CallIndex(rows), rows

    def read(self, rows: list, site: int, offset: int, entry: int = 0x38, base: int = 1, **change) -> dict:
        f = rows[0]
        read = {"site": site, "word": "%08X" % 0, "struct": "rec", "offset": offset, "width": 1,
                "fetch_call": {"fetcher": {"symbol": f["symbol"], "rom_start": f["rom_start"], "size": f["size"],
                                           "sha256": f["sha256"], "load": {"site": 0, "word": "90820000"}},
                               "entry": {"site": entry, "word": "00000000"}, "base": base}}
        read.update(change)
        return read

    def check(self, consumer: list[int], reads: list, fetcher: list[int] | None = None, calls: bool = True,
              tables: list[int] | None = None, record: dict | None = None) -> set:
        """`tables`: the consumer's switch table words (ROM targets) at DATA + 8; `record`: changes to
        its pinned jump_tables record {jr 0x54, site, count, words_sha256} (None: no record)."""
        data, catalogue, index, rows = self.build(consumer, fetcher)
        table = struct.pack(f">{len(tables or [])}I", *(0x80000000 + t for t in tables or []))
        data += table
        for read in reads:
            read["word"] = "%08X" % struct.unpack_from(">I", data, read["site"])[0]
            read["fetch_call"]["entry"]["word"] = "%08X" % struct.unpack_from(">I", data, read["fetch_call"]["entry"]["site"])[0]
            fetch = read["fetch_call"]["fetcher"]
            fetch.update({k: rows[0][k] for k in ("rom_start", "size", "sha256")})
        k = rows[-1]
        owner = {**{x: k[x] for x in ("symbol", "image_id", "rom_start", "size", "sha256")}, "source": "src/k.c", "reads": reads}
        if record is not None:
            owner["jump_tables"] = [{"jr": 0x54, "site": self.DATA + 8, "count": len(tables or []),
                                     "words_sha256": hashlib.sha256(table).hexdigest(), **record}]
        spans = [Span(self.DATA + i, self.DATA + 1 + i, f"rec.f{i % 4}", self.DATA + i - i % 4) for i in range(8)]
        vrams = {(r["symbol"], r["rom_start"], r["size"]): r["vram_start"] for r in rows}
        return assetformat.check_reads(data, [owner], spans, catalogue, vrams, index if calls else None)

    def rows(self, consumer: list[int]) -> list:
        return self.build(consumer)[3]

    def test_derived_offsets_pass(self) -> None:
        straight = [0x24020001, self.jal(0), self.NOP, self.jal(0), self.NOP, self.JR, self.NOP]
        rows = self.rows(straight)
        self.assertEqual(self.check(straight, [self.read(rows, 0x3C, 1), self.read(rows, 0x44, 2)]), {(0x3C, "rec"), (0x44, "rec")})
        helper = [0x24020001, self.jal(self.HELPER), self.NOP, self.jal(0), self.NOP, self.JR, self.NOP]
        self.assertEqual(self.check(helper, [self.read(rows, 0x44, 1)]), {(0x44, "rec")})
        # A forward branch whose two paths fetch equally (bne a0, zero, +2 over one addiu).
        dag = [0x14800002, self.NOP, 0x24420001, self.jal(0), self.NOP, self.JR, self.NOP]
        self.assertEqual(self.check(dag, [self.read(rows, 0x44, 1)]), {(0x44, "rec")})

    def test_every_fetch_rule_fails_closed(self) -> None:
        straight = [0x24020001, self.jal(0), self.NOP, self.jal(0), self.NOP, self.JR, self.NOP]
        rows = self.rows(straight)
        cases = {
            "wrong offset vs count": (straight, [self.read(rows, 0x44, 1)], None, True, "derived fetch position"),
            "entry after site": (straight, [self.read(rows, 0x3C, 1, entry=0x44)], None, True, "before its fetch"),
            "not a jal": (straight, [self.read(rows, 0x40, 1)], None, True, "not a jal to its fetcher"),
            "wrong target": ([0x24020001, self.jal(self.HELPER), self.NOP, self.JR, self.NOP],
                             [self.read(rows, 0x3C, 1)], None, True, "not a jal to its fetcher"),
            "width 2": (straight, [self.read(rows, 0x3C, 1, width=2)], None, True, "plain one-byte fetch"),
            "with frame": (straight, [self.read(rows, 0x3C, 1, frame={"site": 0x38, "word": "24020001"})], None, True,
                           "plain one-byte fetch"),
            "fetcher with two byte loads": (straight, [self.read(rows, 0x3C, 1)], [0x90820000, self.JR, 0x90830001, self.NOP],
                                            True, "exactly the pinned byte load"),
            "missing call index": (straight, [self.read(rows, 0x3C, 1)], None, False, "call index"),
            "closure with a fetch": ([0x24020001, self.jal(self.FETCHING), self.NOP, self.jal(0), self.NOP, self.JR, self.NOP],
                                     [self.read(rows, 0x44, 1)], None, True, "may fetch: func_f is the fetcher"),
            "jalr in a closure": ([0x24020001, self.jal(self.INDIRECT), self.NOP, self.jal(0), self.NOP, self.JR, self.NOP],
                                  [self.read(rows, 0x44, 1)], None, True, "indirect or link call"),
            "unresolved target": ([0x24020001, self.jal(0x1000), self.NOP, self.jal(0), self.NOP, self.JR, self.NOP],
                                  [self.read(rows, 0x44, 1)], None, True, "no unique catalogued target"),
            "backward branch": ([0x24020001, 0x1000FFFE, self.NOP, self.jal(0), self.NOP, self.JR, self.NOP],
                                [self.read(rows, 0x44, 1)], None, True, "does not go forward"),
            "target beyond the site": ([0x10000004, self.NOP, self.NOP, self.jal(0), self.NOP, self.JR, self.NOP],
                                       [self.read(rows, 0x44, 1)], None, True, "does not go forward"),
            "unequal counts on two paths": ([0x14800003, self.NOP, self.jal(0), self.NOP, self.jal(0), self.NOP, self.JR, self.NOP],
                                            [self.read(rows, 0x48, 1)], None, True, "different fetch counts"),
            "likely slot holding a fetch": ([0x50800003, self.jal(0), self.NOP, self.NOP, self.jal(0), self.NOP, self.JR, self.NOP],
                                            [self.read(rows, 0x48, 1)], None, True, "delay slot"),
            "branch from outside into the window": ([0x24020001, self.NOP, self.NOP, self.jal(0), self.NOP, 0x1000FFFC, self.NOP,
                                                     self.JR, self.NOP], [self.read(rows, 0x44, 1)], None, True,
                                                    "branches into the fetch window"),
            "jr in the window": ([self.JR, self.NOP, self.NOP, self.jal(0), self.NOP, self.JR, self.NOP],
                                 [self.read(rows, 0x44, 1)], None, True, "jr, jalr"),
        }
        for name, (consumer, reads, fetcher, calls, message) in cases.items():
            with self.subTest(name), self.assertRaisesRegex(AssetFormatError, message):
                self.check(consumer, reads, fetcher, calls)

    def test_switch_tables_are_derived_and_guard_fetch_windows(self) -> None:
        # Dispatch at 0x38..0x58 (jr v0 at 0x54, table at DATA + 8), then the case with the fetch.
        switch = self.dispatch(7, self.DATA + 8) + [0x24020001, self.jal(0), self.NOP, self.JR, self.NOP]
        rows = self.rows(switch)
        fetch = lambda: [self.read(rows, 0x60, 1, entry=0x5C)]   # noqa: E731
        self.assertEqual(self.check(switch, fetch(), tables=[0x5C, 0x68], record={}), {(0x60, "rec")})
        branch_in = switch[:-2] + [0x1000FFF7, self.NOP]   # 0x68: b 0x48, inside the dispatch
        cases = {"a table entry into the window": (switch, {"tables": [0x5C, 0x60], "record": {}}, "enters the fetch window"),
                 "table target outside the function": (switch, {"tables": [0x5C, 0x200], "record": {}}, "outside its function"),
                 "wrong table site": (switch, {"tables": [0x5C, 0x68], "record": {"site": self.DATA + 12}}, "not the derived"),
                 "count smaller than the bound": (switch, {"tables": [0x5C, 0x68], "record": {"count": 1}}, "not the derived"),
                 "table words changed": (switch, {"tables": [0x5C, 0x68], "record": {"words_sha256": "0" * 64}},
                                         "not the derived"),
                 "jr but no pinned table": (switch, {"tables": [0x5C, 0x68]}, "not one pinned jump table each"),
                 "branch into the dispatch": (branch_in, {"tables": [0x5C, 0x68], "record": {}}, "into the switch dispatch"),
                 "jr that is not a switch": ([0x24020001, 0x00400008] + switch[2:], {"tables": [0x5C, 0x68], "record": {}},
                                             "not a GCC switch dispatch")}
        for name, (consumer, change, message) in cases.items():
            with self.subTest(name), self.assertRaisesRegex(AssetFormatError, message):
                self.check(consumer, fetch(), **change)
        straight = [0x24020001, self.jal(0), self.NOP, self.JR, self.NOP]
        with self.assertRaisesRegex(AssetFormatError, "has no indirect jr"):
            self.check(straight, [self.read(self.rows(straight), 0x3C, 1)], tables=[0x38], record={"jr": 0x3C})

    def test_helper_switches_stay_inside_the_helper(self) -> None:
        for helper, ok in ((self.SWITCHED, True), (self.BAD_JR, False)):
            consumer = [0x24020001, self.jal(helper), self.NOP, self.jal(0), self.NOP, self.JR, self.NOP]
            reads = [self.read(self.rows(consumer), 0x44, 1)]
            if ok:   # a 0x4C4C8-style switch helper is fetch-free
                self.assertEqual(self.check(consumer, reads), {(0x44, "rec")})
            else:
                with self.assertRaisesRegex(AssetFormatError, "may fetch: func_j: jr 0xac is not a GCC switch dispatch"):
                    self.check(consumer, reads)


class SwitchTableTests(unittest.TestCase):
    """mipsflow.switch_table, which assetformat's jump-table and fetch-free checks use: opus-05's
    reviewed GCC jump-table dispatch shape, both bound placements."""
    BASE = 0x80000000

    def table(self, words: list[int], targets: list[int]) -> tuple[bytes, int]:
        code = struct.pack(f">{len(words)}I", *words)
        return code + struct.pack(f">{len(targets)}I", *(self.BASE + t for t in targets)), len(code)

    def dispatch(self, sll_in_slot: bool, table: int, slot: int = 0) -> list[int]:
        head = [0x2C820002, 0x10400008, 0x00041080] if sll_in_slot else [0x2C820002, 0x10400008, slot, 0x00041080]
        return head + [0x3C018000, 0x00220821, 0x8C220000 | table, 0x00400008, 0x00000000, 0x03E00008, 0x00000000]

    def test_both_bound_placements_derive_the_table(self) -> None:
        for in_slot in (True, False):
            words = self.dispatch(in_slot, 0)
            size = 4 * len(words)
            words[-5] = 0x8C220000 | size                  # lw v0, size(at): the table follows the code
            data, low_end = self.table(words, [size - 8, size - 8])
            jr = size - 16
            switch = mipsflow.switch_table(data, 0, low_end, self.BASE, jr)
            self.assertEqual((switch.table, switch.count, switch.targets, switch.bound),
                             (size, 2, (size - 8, size - 8), jr - (0x18 if in_slot else 0x1C)))

    def test_shape_deviations_fail(self) -> None:
        words = self.dispatch(False, 0)
        size = 4 * len(words)
        words[-5] = 0x8C220000 | size
        # Each case: {index: word}. "sll into the table register": sll at, a0, 2; addu at, at, at
        # would address the table at 2 * (hi << 16) + lo at run time, not at the derived (hi << 16) + lo.
        cases = {"slot writes the index": {2: 0x24840001}, "lw into another register": {6: 0x8C230000 | size},
                 "bound on another register": {1: 0x10600008}, "not sltiu": {0: 0x28820002},
                 "sll into the table register": {3: 0x00040880, 5: 0x00210821}}
        for name, change in cases.items():
            bad = list(words)
            for index, word in change.items():
                bad[index] = word
            data, end = self.table(bad, [size - 8, size - 8])
            with self.subTest(name), self.assertRaisesRegex(mipsflow.FlowUnknown, "not a GCC switch dispatch"):
                mipsflow.switch_table(data, 0, end, self.BASE, size - 16)
        data, end = self.table(words, [size - 8, size - 6])
        with self.assertRaisesRegex(mipsflow.FlowUnknown, "outside its function"):
            mipsflow.switch_table(data, 0, end, self.BASE, size - 16)


class TransferWindowTests(unittest.TestCase):
    """A transfer's size argument up to TRANSFER_SIZE_REACH bytes before its jal (opus-10)."""
    WORDS = [0x27BDFFE0,  # 0x00 addiu sp, sp, -0x20
             0x24070008,  # 0x04 addiu a3, zero, 8 (size)
             0x00000000,  # 0x08 nop
             0x00000000,  # 0x0C nop
             0x00000000,  # 0x10 nop
             0x0C040000,  # 0x14 jal (size 0x10 bytes earlier)
             0x00000000,  # 0x18 nop (delay slot)
             0x00000000,  # 0x1C nop
             0x27BD0020]  # 0x20 addiu sp, sp, 0x20

    def check(self, words: dict | None = None, size: int = 4, call: int = 0x14, vram: bool = True) -> set:
        code = list(self.WORDS)
        for at, word in (words or {}).items():
            code[at // 4] = word
        packed = struct.pack(f">{len(code)}I", *code)
        base = len(packed)
        spans = [Span(base + k, base + k + 4, f"rec.f{k}", base) for k in (0, 4)]
        key = ("func_0", 0, len(packed))
        consumer = {"symbol": "func_0", "image_id": "main_14400", "rom_start": 0, "size": len(packed),
                    "sha256": hashlib.sha256(packed).hexdigest(), "source": "src/x.c",
                    "reads": [{"site": size, "word": f"{code[size // 4]:08X}", "struct": "rec", "offset": 0, "width": 8,
                               "transfer_call": {"site": call, "word": f"{code[call // 4]:08X}"}}]}
        return assetformat.check_reads(packed + bytes(range(1, 9)), [consumer], spans,
                                       {key: consumer["sha256"]}, {key: 0x80100000} if vram else None)

    def test_straight_line_size_up_to_five_instructions_back_passes(self) -> None:
        self.assertEqual(self.check(), {(4, "rec")})
        # An adjacent size argument passes under the same liveness proof.
        self.assertEqual(self.check({0x04: 0, 0x10: 0x24070008}, size=0x10), {(0x10, "rec")})

    def test_unsafe_or_distant_size_arguments_fail(self) -> None:
        cases = {
            "six instructions back": ({0x14: 0, 0x1C: 0x0C040000}, 0x1C, "not next to its pinned call"),
            "size register rewritten": ({0x0C: 0x24070004}, 0x14, "unchanged on every path"),
            "branch between": ({0x08: 0x10000001}, 0x14, "unchanged on every path"),
            "call between": ({0x0C: 0x0C050000}, 0x14, "unchanged on every path"),
            "branch into the window": ({0x1C: 0x1000FFFB}, 0x14, "unchanged on every path"),
            "indirect jump hides the flow": ({0x1C: 0x00800008}, 0x14, "unchanged on every path"),
        }
        for name, (words, call, message) in cases.items():
            with self.subTest(name), self.assertRaisesRegex(AssetFormatError, message):
                self.check(words, call=call)
        with self.assertRaisesRegex(AssetFormatError, "unchanged on every path"):  # no vram: j targets unknown
            self.check({0x1C: 0x08000000}, vram=False)

    def test_sol_f2_size_liveness_through_the_delay_slot(self) -> None:
        # SolReviewAssetStack F2 P2-1, exact words: size 0x04=24070008, JAL 0x14, delay slot 0x18=24070004.
        with self.assertRaisesRegex(AssetFormatError, "unchanged on every path"):
            self.check({0x18: 0x24070004})
        # The adjacent form gets the same proof: its slot, a loop back to the call that skips the size,
        # and a function whose flow graph is unknown all fail closed (the adjacent rule used to need none).
        adjacent = {0x04: 0, 0x10: 0x24070008}
        for name, (words, vram) in {"slot rewrites the size": ({0x18: 0x24070004}, True),
                                    "loop back to the call": ({0x1C: 0x1000FFFD}, True),
                                    "indirect jump hides the flow": ({0x1C: 0x00800008}, False)}.items():
            with self.subTest(name), self.assertRaisesRegex(AssetFormatError, "unchanged on every path"):
                self.check({**adjacent, **words}, size=0x10, vram=vram)


@unittest.skipUnless(PRIVATE_ROM.is_file(), "Private copied original required")
class CodeBasedWitnessTests(unittest.TestCase):
    """opus-05: glyph/icon banks, message bank, script bank, floor tables and the segment-6 TLUT
    (opus-10's SegmentTwoBankTests cover the sub-ranges of B_194fc0)."""
    ROWS = {"B_157b80": (0x157B80, 0x175150), "B_175150": (0x175150, 0x177150), "B_177150": (0x177150, 0x179560),
            "B_179560": (0x179560, 0x194FC0), "B_1c6cb0": (0x1C6CB0, 0x23E210), "B_23e210": (0x23E210, 0x240C20),
            "N_palette_e7be70": (0xE7BE70, 0xE7BE90)}
    OPEN: dict = {}  # B_194fc0 is split by opus-10 (SegmentTwoBankTests)
    # Handler jump tables (ROM site, entries, first case) whose case targets are the fetch_call entries.
    JUMP_TABLES = {"func_801EBCA8_13DBC8": (0x147DF8, 14, 0x02), "func_801EB728_13D648": (0x147DB8, 14, 0x52),
                   "func_801EDBFC_13FB1C": (0x147EF0, 11, 0x30), "func_801EDFBC_13FEDC": (0x147F20, 21, 0x3B),
                   "func_801F45F4_146514": (0x1483B0, 28, 0x64), "func_801F3664_145584": (0x1482F0, 17, 0x65),
                   "func_801F3B5C_145A7C": (0x148338, 30, 0x60)}

    @classmethod
    def setUpClass(cls) -> None:
        cls.original = PRIVATE_ROM.read_bytes()
        cls.rules = json.loads((PROJECT / rommap.RULES_PATH).read_text())
        cls.evidence = rommap.load_evidence(cls.original, cls.rules)
        cls.document = cls.evidence[assetformat.INPUT]
        generated = json.loads((PROJECT / rommap.MAP_PATH).read_text())
        cls.rows = {(r[0], r[1]): (r[2], r[3]) for r in generated["ranges"]}
        cls.base_rows = {(r[0], r[1]): (r[2], r[3])
                         for r in rommap.unsplit_rows(generated["ranges"], generated.get("splits", []))}
        cls.words = rommap.owner_words(cls.original, cls.rules, cls.evidence)
        cls.functions = json.loads((PROJECT / rommap.CATALOGUE_PATH).read_text())["identity"]["functions"]
        cls.witness = {w["ranges"][0]["ref"]: w for w in cls.document["witnesses"] if w["id"].startswith("codebased_")}

    def patched(self, at: int, value: int, width: int = 1) -> bytes:
        data = bytearray(self.original)
        data[at:at + width] = value.to_bytes(width, "big")
        return bytes(data)

    def typed(self, ref: str, original: bytes | None = None) -> list:
        data = original or self.original
        w = self.witness[ref]
        interval = self.ROWS[ref]
        context = tuple((c["start"], c["end"]) for c in w["context"])
        spans = assetformat.PARSERS[w["format"]](data, (interval,), {**w["params"], "context": context})
        return assetformat.check_tiling(data, spans, [interval])

    def replay(self, document: dict, original: bytes | None = None) -> dict:
        return assetformat.replay(original or self.original, document, self.functions, self.rows,
                                  rommap.immediate_at, rommap.EXPECTED_SHA256,
                                  {row["id"]: row for row in self.rules["tables"]}, self.words, self.base_rows)

    def test_seven_code_based_rows_are_witnessed(self) -> None:
        closed = self.replay(self.document)
        for ref, interval in self.ROWS.items():
            self.assertEqual(self.rows[interval], ("asset_raw_code_based", ref))
            self.assertTrue(closed[interval].startswith("codebased_"))
        for ref, interval in self.OPEN.items():
            self.assertEqual(self.rows[interval], ("asset_raw_code_based", ref))
            self.assertNotIn(interval, closed)
        self.assertEqual(sum(b - a for a, b in self.ROWS.values()), 750544)
        rom_map = rommap.load(PROJECT, self.original)
        self.assertTrue(frozenset(self.ROWS.values()) <= rom_map.format_witnessed)
        self.assertFalse(frozenset(self.OPEN.values()) & rom_map.format_witnessed)
        result = rommap.account(rom_map, self.original, PROJECT, [], self.rules)
        statuses = {(r["start"], r["end"]): r["status"] for r in result["range_statuses"]}
        for interval in self.ROWS.values():
            self.assertEqual(statuses[interval], "classified_binary")
        for interval in self.OPEN.values():
            self.assertEqual(statuses[interval], "located")

    def script_bodies(self) -> list[tuple[int, int]]:
        start, end = self.ROWS["B_1c6cb0"]
        offsets = [struct.unpack_from(">I", self.original, start + 4 * i)[0]
                   for i in range(struct.unpack_from(">I", self.original, start)[0] // 4)]
        return [(start + a, start + b) for a, b in zip(offsets, offsets[1:] + [end - start])]

    def test_every_script_body_walks_to_its_final_return(self) -> None:
        spans = self.typed("B_1c6cb0")
        ends = {s.end: s for s in spans if s.field != PAD}
        for a, b in self.script_bodies():
            last = b
            while last not in ends:
                last -= 1
            self.assertEqual((ends[last].field, ends[last].start), ("script_op_03.opcode", last - 1))
            self.assertFalse(any(self.original[last:b]))
            self.assertLess(b - last, 16 if b == self.ROWS["B_1c6cb0"][1] else 4)
        self.assertEqual(sum(s.end - s.start for s in spans if s.field == PAD), 2224)
        self.assertEqual(sum(1 for s in spans if s.field.endswith(".opcode") or s.field in
                             ("script_char.code", "script_kanji.lead")), 301126 - 2224)

    def test_script_unknown_opcode_overrun_missing_return_and_width_tamper_fail(self) -> None:
        spans = self.typed("B_1c6cb0")
        starts = {s.start for s in spans if s.field.endswith(".opcode") or s.field in ("script_char.code", "script_kanji.lead")}
        plain = next(s.start for s in spans if s.field == "script_op_10.opcode")
        end = self.ROWS["B_1c6cb0"][1]
        ret = next(b - 1 for a, b in self.script_bodies() if self.original[b - 1] == 3 and b < end)
        # A body ending in a zero-argument instruction and its return: widening the former to a
        # one-byte load swallows the return.
        widen = next(b - 2 for a, b in self.script_bodies() if b < end and self.original[b - 1] == 3 and b - 2 in starts
                     and self.original[b - 2] < 0x80 and self.original[b - 2] != 0x1D
                     and not assetformat._SCRIPT_ARGS.get(self.original[b - 2], 0))
        # A conditional whose jump offset is loaded just before it; one more lands inside an instruction.
        load = next(s.start for s in spans if s.field == "script_op_5c.opcode"
                    and self.original[s.start + 2] in assetformat._SCRIPT_CONDS and self.original[s.start + 1] < 0x7F
                    and s.start + 2 + self.original[s.start + 1] + 1 not in starts)
        cases = {"unknown opcode": (self.patched(plain, 0x52), "no pinned handler reads"),
                 "overrun past the body": (self.patched(ret, 0x08), "runs past body"),
                 "missing terminator": (self.patched(ret, 0x1E), "without return opcode 3"),
                 "operand width": (self.patched(widen, 0x50), "without return opcode 3"),
                 "jump offset": (self.patched(load + 1, self.original[load + 1] + 1), "is not an instruction of body")}
        for name, (data, message) in cases.items():
            with self.subTest(name), self.assertRaisesRegex(AssetFormatError, message):
                self.typed("B_1c6cb0", data)
        # A wrong argument width for a frequent opcode desynchronizes the walk. (A rare one, such as
        # 0x08 with 6 instances, may resynchronize: the widths rest on the derived fetch pins.)
        for op, width in ((0x3D, 2), (0x26, 1), (0x50, 2)):
            with self.subTest(op=op), mock.patch.dict(assetformat._SCRIPT_ARGS, {op: width}), \
                    self.assertRaises(AssetFormatError):
                self.typed("B_1c6cb0")

    def test_script_fetch_pins_are_derived_attributed_and_case_entries(self) -> None:
        fmt = self.document["formats"]["script_bank"]
        params = self.witness["B_1c6cb0"]["params"]
        reads = [(c["symbol"], r) for c in fmt["consumers"] for r in c["reads"] if "fetch_call" in r]
        labels = {site: label for site, label in params["fetch"]["sites"]}
        pinned = {}
        for symbol, r in reads:
            field = labels[r["site"]]
            pinned.setdefault(r["site"], set()).add(r["struct"])
            self.assertTrue(field == "dispatch:opcode" or r["struct"] in field.replace(".", " ").split(), (r["site"], field))
        self.assertEqual({s for s, label in labels.items() if ":" not in label or label == "dispatch:opcode"}, set(pinned))
        self.assertEqual(pinned[0x1410F0], {f"script_op_{op:02x}" for op in params["opcodes"]} | {"script_char", "script_kanji"})
        tables = {c["symbol"]: [(t["site"], t["count"]) for t in c["jump_tables"]] for c in fmt["consumers"] if "jump_tables" in c}
        self.assertEqual(tables, {**{symbol: [(site, count)] for symbol, (site, count, _) in self.JUMP_TABLES.items()},
                                  "func_801F45F4_146514": [(0x1483B0, 28), (0x148420, 16)]})
        overlay = [f for f in self.functions if f["image_id"] == "overlay_136dc0"]
        self.assertEqual(params["fetch"]["text"], [min(f["rom_start"] for f in overlay),
                                                   max(f["rom_start"] + f["size"] for f in overlay)])
        names = {f["symbol"]: f for f in overlay}
        caller, callee, call = names["func_801EF7F0_141710"], names["func_801EB208_13D128"], params["name_call"]
        self.assertEqual(call["jtbl_site"], 0x148028 + 4 * (call["function"] - 1))   # func_801EF7F0 jtbl, cases 1..0x83
        self.assertEqual(int(call["jtbl_word"], 16) - caller["vram_start"] + caller["rom_start"], call["call_site"])
        self.assertEqual(int(call["call_word"], 16), 0x0C000000 | (callee["vram_start"] >> 2 & 0x3FFFFFF))
        for symbol, r in reads:
            if symbol in self.JUMP_TABLES and r["struct"].startswith("script_op_"):
                table, entries, first = self.JUMP_TABLES[symbol]
                row = next(f for f in self.functions if f["symbol"] == symbol)
                op = int(r["struct"][-2:], 16)
                target = struct.unpack_from(">I", self.original, table + 4 * (op - first))[0]
                self.assertEqual(r["fetch_call"]["entry"]["site"], target - row["vram_start"] + row["rom_start"], r["struct"])
        document = copy.deepcopy(self.document)
        witness = next(w for w in document["witnesses"] if w["id"] == self.witness["B_1c6cb0"]["id"])
        witness["params"]["fetch"]["sites"].pop()
        with self.assertRaisesRegex(AssetFormatError, "every fetcher call exactly once"):
            self.replay(document)
        for change in ({"offset": 3}, {"base": 2}):
            document = copy.deepcopy(self.document)
            read = next(r for c in document["formats"]["script_bank"]["consumers"] for r in c["reads"]
                        if r["struct"] == "script_op_08" and r["offset"] == 2)
            if "base" in change:
                read["fetch_call"]["base"] = change["base"]
            else:
                read["offset"] = change["offset"]
            with self.subTest(change), self.assertRaisesRegex(AssetFormatError, "derived fetch position"):
                self.replay(document)
        document = copy.deepcopy(self.document)
        next(c for c in document["formats"]["script_bank"]["consumers"] if c["symbol"] == "func_801F3B5C_145A7C").pop("jump_tables")
        with self.assertRaisesRegex(AssetFormatError, "not one pinned jump table each"):
            self.replay(document)

    def test_glyph_box_count_and_header_tamper_fail(self) -> None:
        a = self.ROWS["B_157b80"][0]
        cases = [self.patched(a + 2 + 7, 0x01),           # pixel column 15 of glyph 0 (width 6)
                 self.patched(a, 17),                       # width beyond the 16-pixel row
                 self.patched(a + 1, 13),                   # height not in the reviewed set
                 self.patched(self.ROWS["B_157b80"][1] - 1, 1)]   # nonzero fill
        for data in cases:
            with self.assertRaises(AssetFormatError):
                self.typed("B_157b80", data)
        icon = self.ROWS["B_177150"][0]
        with self.assertRaises(AssetFormatError):
            self.typed("B_177150", self.patched(icon + 2 + 7, 0x01))   # column 15 > width 12 + shadow 1

    def test_message_trail_offset_and_limit_tamper_fail(self) -> None:
        a = self.ROWS["B_179560"][0]
        lead = next(p for p in range(a, a + 64) if self.original[p] & 0xF0 == 0xF0)
        table = 0x120470
        second = struct.unpack_from(">I", self.original, table + 4)[0]
        cases = [self.patched(lead + 1, 0),                 # lead without trail
                 self.patched(table + 4, second + 1, 4),     # offset inside a message
                 self.patched(0x1B4E4, 0x2C825DC0, 4),      # different id limit
                 self.patched(self.ROWS["B_179560"][1] - 2, 0x41)]   # unreferenced trailing text
        for data in cases:
            with self.assertRaises(AssetFormatError):
                self.typed("B_179560", data)

    def test_floor_table_tamper_fail(self) -> None:
        f = self.ROWS["B_23e210"][0]
        keys = f + struct.unpack_from(">I", self.original, f)[0]
        end = next(p for p in range(keys, keys + 400, 4) if self.original[p] == 0)
        count = struct.unpack_from(">H", self.original, end + 2)[0]
        event, placement = (f + struct.unpack_from(">I", self.original, f + 8 * i + 4)[0] for i in (0, 1))
        for data in (self.patched(end + 2, count + 1, 2),     # record count no longer fills the block
                     self.patched(keys + 1, 1),               # nonzero key padding byte
                     self.patched(f + 4, struct.unpack_from(">I", self.original, f + 4)[0] + 4, 4),
                     self.patched(event + 0xB, 1),            # nonzero event alignment members
                     self.patched(event + 0x15, 1),
                     self.patched(placement + 7, 1),          # nonzero placement alignment members
                     self.patched(placement + 0xB, 1)):
            with self.assertRaises(AssetFormatError):
                self.typed("B_23e210", data)
        w = self.witness["B_23e210"]
        for params in ({**w["params"], "record_sizes": [22, 11]}, {**w["params"], "record_structs": ["floor_event", "rec"]}):
            with self.subTest(params), self.assertRaisesRegex(AssetFormatError, "layout"):
                assetformat.PARSERS["floor_event_tables"](self.original, (self.ROWS["B_23e210"],), {**params, "context": ()})

    def test_every_floor_record_member_has_a_pinned_consumer_load(self) -> None:
        consumers = self.document["formats"]["floor_event_tables"]["consumers"]
        loads = [r for c in consumers for r in c["reads"] if "transfer_call" not in r]
        for name, layout in assetformat._FLOOR_LAYOUTS.items():
            covered = {k for r in loads if r["struct"] == name for k in range(r["offset"], r["offset"] + r["width"])}
            at = 0
            for member, width in layout:
                field, extent = f"{name}.{member}", set(range(at, at + width))
                if field in assetformat.REQUIRED_ZERO:
                    self.assertFalse(extent & covered, field)    # alignment members are never loaded
                else:
                    self.assertTrue(extent <= covered, field)
                at += width
        # Stack-copy loads only count through their frame: without it the immediates disagree.
        document = copy.deepcopy(self.document)
        for consumer in document["formats"]["floor_event_tables"]["consumers"]:
            for read in consumer["reads"]:
                read.pop("frame", None)
        with self.assertRaisesRegex(AssetFormatError, "offset disagrees"):
            self.replay(document)

    def test_tlut_entry_count_and_shifted_rows_fail(self) -> None:
        with self.assertRaises(AssetFormatError):
            self.typed("N_palette_e7be70", self.patched(0x434B0, 0x354A8000, 4))   # LoadTLUT of 15 entries
        for ref in ("B_175150", "N_palette_e7be70"):
            document = copy.deepcopy(self.document)
            for w in document["witnesses"]:
                if w["id"] == self.witness[ref]["id"]:
                    w["ranges"][0]["end"] += 4
            with self.subTest(ref), self.assertRaises(AssetFormatError):
                self.replay(document)


@unittest.skipUnless(PRIVATE_ROM.is_file(), "Private copied original required")
class LibmusDualReadWitnessTests(unittest.TestCase):
    """opus-08 wave 2 layer B r3: songs closed with a declared level over-read into the master stream."""
    SONG = (0xD72020, 0xD73370)      # channel 11 volume reads the master's 85 FF 60 as one event
    SEEK = (0xDF04C0, 0xDF0770)      # over-reads only in func_80129F10 marker-0 starts (95 FF 85, one tick)

    @classmethod
    def setUpClass(cls) -> None:
        cls.original = PRIVATE_ROM.read_bytes()
        cls.rules = json.loads((PROJECT / rommap.RULES_PATH).read_text())
        cls.document = rommap.load_evidence(cls.original, cls.rules)[assetformat.INPUT]
        cls.rows = {(r[0], r[1]): (r[2], r[3]) for r in json.loads((PROJECT / rommap.MAP_PATH).read_text())["ranges"]}
        cls.functions = json.loads((PROJECT / rommap.CATALOGUE_PATH).read_text())["identity"]["functions"]
        cls.witness = next(w for w in cls.document["witnesses"] if w["id"] == f"libmus_song_{cls.SONG[0]:07x}")

    def u32(self, at: int) -> int:
        return struct.unpack_from(">I", self.original, at)[0]

    def patched(self, at: int, value: int, width: int = 1) -> bytes:
        data = bytearray(self.original)
        data[at:at + width] = value.to_bytes(width, "big")
        return bytes(data)

    def typed(self, params: dict, original: bytes | None = None) -> list:
        data = original or self.original
        spans = assetformat.PARSERS["libmus_song"](data, (self.SONG,), {**params, "context": ()})
        return assetformat.check_tiling(data, spans, [self.SONG])

    def declared(self, change=None) -> dict:
        params = copy.deepcopy(self.witness["params"])
        if change:
            change(params["dual_read"][0])
        return params

    def replay(self, document: dict) -> dict:
        return assetformat.replay(self.original, document, self.functions, self.rows, rommap.immediate_at,
                                  rommap.EXPECTED_SHA256, {row["id"]: row for row in self.rules["tables"]},
                                  owner_words(self.original, self.rules), map_base_rows())

    def test_dual_read_songs_are_witnessed_with_their_declaration(self) -> None:
        closed = self.replay(self.document)
        dual = [w for w in self.document["witnesses"] if w["format"] == "libmus_song" and w["params"]]
        self.assertEqual(len(dual), 57)
        causes = [r["cause"] for w in dual for r in w["params"]["dual_read"]]
        self.assertEqual([causes.count(c) for c in ("loop_restores_after_level_data", "restore_keeps_level_clock",
                                                    "seek_update_runs_one_tick")], [45, 10, 2])
        self.assertEqual(sum(w["ranges"][0]["end"] - w["ranges"][0]["start"] for w in dual), 678048)
        for w in dual:
            interval = (w["ranges"][0]["start"], w["ranges"][0]["end"])
            self.assertEqual(closed[interval], w["id"])
            for record in w["params"]["dual_read"]:
                self.assertEqual(record["boundary"], self.u32(interval[0] + 0x24))
                if record["cause"] == "loop_restores_after_level_data":
                    self.assertGreater(record["voice_span_ticks"], record["level_ticks"])
                elif record["cause"] == "restore_keeps_level_clock":
                    self.assertEqual(record["voice_span_ticks"], record["level_ticks"])
                    self.assertTrue(any(key.startswith("ee0/") for key in record["reads"]))
                    self.assertLessEqual(max(record["reads"].values()) - record["level_ticks"], 2)
                else:
                    self.assertEqual(record["cause"], "seek_update_runs_one_tick")
                    self.assertEqual(record["voice_span_ticks"], record["level_ticks"])
                    self.assertTrue(all(key.startswith("marker_") for key in record["reads"]))
                    self.assertEqual(set(record["reads"].values()), {record["level_ticks"] + 1})
                last = record["events"][-1]
                self.assertGreater(last["read_step"] + last["wait"], max(record["reads"].values()))
        record = self.witness["params"]["dual_read"][0]
        master = self.SONG[0] + self.u32(self.SONG[0] + 0x24)
        self.assertEqual(self.original[master:master + 3].hex(), "85ff60")
        self.assertEqual((record["channel"], record["level"]), (11, "volume"))
        self.assertEqual(record["events"], [{"offset": master - self.SONG[0], "bytes": "85ff60", "value": 5,
                                             "wait": 32610, "read_step": record["level_ticks"] + 1}])
        seek = next(w for w in dual if w["id"] == f"libmus_song_{self.SEEK[0]:07x}")["params"]["dual_read"]
        head = self.SEEK[0] + self.u32(self.SEEK[0] + 0x24)
        self.assertEqual(len(seek), 1)
        self.assertEqual(sorted(seek[0]["reads"]), ["marker_0/next_frame", "marker_0/same_frame"])
        self.assertEqual([ev["bytes"] for ev in seek[0]["events"]], [self.original[head:head + 3].hex()])
        self.assertEqual(self.original[head:head + 3].hex(), "95ff85")

    def test_undeclared_or_wrong_dual_read_is_refused(self) -> None:
        with self.assertRaisesRegex(AssetFormatError, "reads past its streams into master region"):
            self.typed({})
        self.assertTrue(self.typed(self.witness["params"]))
        cases = {
            "fewer bytes": lambda r: r["events"][0].update(bytes="85ff"),
            "another wait": lambda r: r["events"][0].update(wait=32609),
            "an extra event": lambda r: r["events"].append(dict(r["events"][0], offset=r["events"][0]["offset"] + 3)),
            "another voice": lambda r: r.update(channel=r["channel"] - 1),
            "another reach": lambda r: r["reads"].update({k: v + 1 for k, v in r["reads"].items()}),
            "a start it does not happen in": lambda r: r["reads"].update({"marker_0/same_frame": 1}),
            "another span": lambda r: r.update(voice_span_ticks=r["voice_span_ticks"] + 1),
            "another cause": lambda r: r.update(cause="restore_keeps_level_clock"),
        }
        for name, change in cases.items():
            with self.subTest(name), self.assertRaises(AssetFormatError):
                self.typed(self.declared(change))

    def test_dual_read_outside_the_documented_bytes_is_refused(self) -> None:
        master = self.SONG[0] + self.u32(self.SONG[0] + 0x24)
        # 85 01 ..: the same first event with a 3-tick wait, so the voice reads on into bytes the
        # declaration does not name (the master stream still decodes: 0x85 takes one operand).
        with self.assertRaisesRegex(AssetFormatError, "declared dual_read is not the over-read"):
            self.typed(self.witness["params"], self.patched(master + 1, 0x01))

    def test_a_boundary_that_would_have_to_move_is_refused(self) -> None:
        def earlier(r):
            r["boundary"] -= 3
            r["events"][0]["offset"] -= 3
        for name, change in (("boundary before the master", earlier),
                             ("boundary after the master", lambda r: r.update(boundary=r["boundary"] + 3)),
                             ("region", lambda r: r.update(level_region=r["level_region"] + 1))):
            with self.subTest(name), self.assertRaises(AssetFormatError):
                self.typed(self.declared(change))
        # Moving the header's master pointer past the over-read bytes (so the level region would
        # absorb them) changes the regions the header defines: the declaration no longer holds.
        with self.assertRaises(AssetFormatError):
            self.typed(self.witness["params"], self.patched(self.SONG[0] + 0x24, self.u32(self.SONG[0] + 0x24) + 3, 4))

    def test_record_tamper_fails_closed(self) -> None:
        document = copy.deepcopy(self.document)
        for w in document["witnesses"]:
            if w["id"] == self.witness["id"]:
                w["params"] = {}
        with self.assertRaises((AssetFormatError, rommap.MapError)):
            self.replay(document)


@unittest.skipUnless(PRIVATE_ROM.is_file(), "Private copied original required")
class SceneT2WitnessTests(unittest.TestCase):
    """opus-08 T2: scene map files and sky files of D_8013BAD4/D_8013C084."""
    MAP = (0x30DD90, 0x313AE0)       # one section, a room, patches, a float grid
    ZERO_MAPS = ((0x4B7870, 0x4BA8C0), (0x8A82C0, 0x8AB7B0))   # all-zero grid: below the 1/4 evidence share
    MAPS = (0x5A04F0, 0x5BD4C0)      # ten sections
    SKY = (0x49EFB0, 0x4A1770)       # 160x60 backdrop, 256-entry TLUT
    SKY_0C = (0x32C8B0, 0x32DD10)    # backdrop +0xC = 0xB000 (never read): not witnessed; 176-entry TLUT
    SKIES_0C = ((0x32C8B0, 0x32DD10), (0x3DDEF0, 0x3E1800), (0x415F70, 0x419350), (0x458A90, 0x459170),
                (0x4778E0, 0x47B2A0))
    TINY = (0x33A4C0, 0x33A4E0)      # 32-byte sky file without backdrop

    @classmethod
    def setUpClass(cls) -> None:
        cls.original = PRIVATE_ROM.read_bytes()
        cls.rules = json.loads((PROJECT / rommap.RULES_PATH).read_text())
        document = rommap.load_evidence(cls.original, cls.rules)[assetformat.INPUT]
        cls.document = {**document, "formats": {k: v for k, v in document["formats"].items() if k.startswith("scene_")},
                        "witnesses": [w for w in document["witnesses"] if w["format"].startswith("scene_")]}
        cls.rows = {(r[0], r[1]): (r[2], r[3]) for r in json.loads((PROJECT / rommap.MAP_PATH).read_text())["ranges"]}
        cls.functions = json.loads((PROJECT / rommap.CATALOGUE_PATH).read_text())["identity"]["functions"]

    def u32(self, at: int) -> int:
        return struct.unpack_from(">I", self.original, at)[0]

    def patched(self, at: int, value: int, width: int = 1) -> bytes:
        data = bytearray(self.original)
        data[at:at + width] = value.to_bytes(width, "big")
        return bytes(data)

    def typed(self, fmt: str, interval, original: bytes | None = None) -> list:
        data = original or self.original
        spans = assetformat.PARSERS[fmt](data, (interval,), {"context": ()})
        return assetformat.check_tiling(data, spans, [interval])

    def replay(self, document: dict, original: bytes | None = None) -> dict:
        return assetformat.replay(original or self.original, document, self.functions, self.rows,
                                  rommap.immediate_at, rommap.EXPECTED_SHA256,
                                  {row["id"]: row for row in self.rules["tables"]},
                                  owner_words(original or self.original, self.rules), map_base_rows())

    def test_map_and_backdrop_sky_files_are_witnessed(self) -> None:
        closed = self.replay(self.document)
        t2 = {k for k, (cls, ref) in self.rows.items() if ref == "T2" and cls == "asset_raw_referenced"}
        self.assertEqual(len(t2), 86)
        self.assertEqual(len(closed), 46)
        self.assertTrue(set(closed) <= t2)
        self.assertEqual(sum(1 for v in closed.values() if v.startswith("scene_map_")), 41)
        self.assertEqual(sum(b - a for a, b in t2 - set(closed)), 33 * 32 + 12368 + 13552 + 49648)
        self.assertNotIn(self.TINY, closed)
        with self.assertRaisesRegex(AssetFormatError, "not positively typed"):
            self.typed("scene_sky", self.TINY)
        for interval in self.ZERO_MAPS:
            self.assertNotIn(interval, closed)
            with self.subTest(interval=interval), self.assertRaisesRegex(AssetFormatError, "dominated by zero"):
                self.typed("scene_map", interval)
        for interval in self.SKIES_0C:
            self.assertNotIn(interval, closed)
            with self.subTest(interval=interval), self.assertRaisesRegex(AssetFormatError, r"\+0xC holds"):
                self.typed("scene_sky", interval)

    def test_nonzero_backdrop_reserve_skies_stay_located(self) -> None:
        rom_map = rommap.load(PROJECT, self.original)
        result = rommap.account(rom_map, self.original, PROJECT, [], self.rules)
        statuses = {(r["start"], r["end"]): r["status"] for r in result["range_statuses"]}
        for interval in self.SKIES_0C:
            with self.subTest(interval=interval):
                self.assertEqual(self.rows[interval], ("asset_raw_referenced", "T2"))
                self.assertNotIn(interval, rom_map.format_witnessed)
                self.assertEqual(statuses[interval], "located")

    def test_unread_backdrop_member_must_be_zero(self) -> None:
        s = self.SKY_0C[0]
        backdrop = s + (self.u32(s + (self.u32(s) & 0xFFFFFF)) & 0xFFFFFF)
        texels = s + (self.u32(backdrop) & 0xFFFFFF)
        zeroed = bytearray(self.original)
        zeroed[backdrop + 0xC] = 0
        self.assertTrue(self.typed("scene_sky", self.SKY_0C, bytes(zeroed)))
        zeroed[texels] = 0xFF                   # 176-entry TLUT: index 255 is past it
        with self.assertRaisesRegex(AssetFormatError, "past the 176-entry TLUT"):
            self.typed("scene_sky", self.SKY_0C, bytes(zeroed))

    def test_map_and_sky_tamper_fail(self) -> None:
        m, s = self.MAP[0], self.SKY[0]
        section = m + (self.u32(m + 4) & 0xFFFFFF)
        cells = m + (self.u32(section) & 0xFFFFFF)
        menu = m + (self.u32(m + 8) & 0xFFFFFF)
        info = m + (self.u32(menu) & 0xFFFFFF)
        rooms = m + (self.u32(info + 0x10) & 0xFFFFFF)
        sky = s + (self.u32(s) & 0xFFFFFF)
        cases = {
            "section count": ("scene_map", self.MAP, self.patched(m + 3, 2)),
            "section end": ("scene_map", self.MAP, self.patched(m + 15, self.original[m + 15] + 8)),
            "section gap": ("scene_map", self.MAP, self.patched(self.MAP[1] - 9, 1)),
            "cell count": ("scene_map", self.MAP, self.patched(section + 7, self.original[section + 7] + 1)),
            "cell alignment": ("scene_map", self.MAP, self.patched(cells + 3, 1)),
            "cell model segment": ("scene_map", self.MAP, self.patched(cells + 0x28, 5)),
            "map width": ("scene_map", self.MAP, self.patched(info + 3, 57)),
            "room count high": ("scene_map", self.MAP, self.patched(info + 0xC, 1)),
            "room side high": ("scene_map", self.MAP, self.patched(rooms + 0x10, 1)),
            "menu segment": ("scene_map", self.MAP, self.patched(menu + 0x10, 6)),
            "sky header fill": ("scene_sky", self.SKY, self.patched(s + 7, 1)),
            "backdrop reserve": ("scene_sky", self.SKY, self.patched(sky - 0x18 + 0xC, 1)),
            "band fill": ("scene_sky", self.SKY, self.patched(sky + 9, 1)),
            "sky tail": ("scene_sky", self.SKY, self.patched(self.SKY[1] - 1, 1)),
        }
        for name, (fmt, interval, data) in cases.items():
            with self.subTest(name), self.assertRaises(AssetFormatError):
                self.typed(fmt, interval, data)
        for fmt, interval in (("scene_map", self.MAP), ("scene_map", self.MAPS), ("scene_sky", self.SKY)):
            self.assertTrue(self.typed(fmt, interval))

    def test_shifted_or_swapped_intervals_fail(self) -> None:
        for fmt, ranges in (("scene_map", ((self.MAP[0], self.MAP[1] - 16),)),
                            ("scene_map", ((self.MAP[0], self.MAP[1] + 16),)),
                            ("scene_map", (self.MAP, self.MAPS)), ("scene_sky", (self.MAP,)),
                            ("scene_map", (self.SKY,)), ("scene_sky", ((self.SKY[0], self.SKY[1] + 16),))):
            with self.subTest(fmt=fmt, ranges=ranges), self.assertRaises(AssetFormatError):
                spans = assetformat.PARSERS[fmt](self.original, ranges, {"context": ()})
                assetformat.check_tiling(self.original, spans, list(ranges))

    def test_record_tamper_fails_closed(self) -> None:
        def edit(change) -> dict:
            document = copy.deepcopy(self.document)
            change(document)
            return document
        mapw = lambda d: next(w for w in d["witnesses"] if w["format"] == "scene_map")  # noqa: E731
        skyw = lambda d: next(w for w in d["witnesses"] if w["format"] == "scene_sky")  # noqa: E731
        reads = lambda d: d["formats"]["scene_map"]["consumers"][0]["reads"]  # noqa: E731
        cases = {
            "map digest": lambda d: mapw(d).update(span_digest="0" * 64),
            "sky count": lambda d: skyw(d).update(span_count=skyw(d)["span_count"] + 1),
            "anchor pair": lambda d: mapw(d)["anchors"][0].update(site=mapw(d)["anchors"][0]["site"] + 16),
            "anchor table": lambda d: mapw(d)["anchors"][0].update(table="T6"),
            "no start anchor": lambda d: skyw(d).update(anchors=[a for a in skyw(d)["anchors"] if a["edge"] == "end"]),
            "read offset": lambda d: reads(d)[0].update(offset=8),
            "read struct": lambda d: reads(d)[4].update(struct="scene_mark"),
            "params": lambda d: mapw(d).update(params={"strict": True}),
            "wrong class": lambda d: skyw(d)["ranges"][0].update(**{"class": "asset_yay0"}),
        }
        for name, change in cases.items():
            with self.subTest(name), self.assertRaises((AssetFormatError, rommap.MapError)):
                self.replay(edit(change))


@unittest.skipUnless(PRIVATE_ROM.is_file(), "Private copied original required")
class T4WitnessTests(unittest.TestCase):
    PICTURE = (0x857020, 0x85B870)  # T4 record 1 picture: file_size 18,504, work_units 13,554
    MASK = (0x85B870, 0x85B920)     # T4 record 1 mask: rle_bytes 151
    BLANK = (0x895F80, 0x896060)    # T4 record 14: the 212-byte blank picture shared by 9 records

    @classmethod
    def setUpClass(cls) -> None:
        cls.original = PRIVATE_ROM.read_bytes()
        cls.rules = json.loads((PROJECT / rommap.RULES_PATH).read_text())
        cls.document = rommap.load_evidence(cls.original, cls.rules)[assetformat.INPUT]
        cls.rows = {(r[0], r[1]): (r[2], r[3]) for r in json.loads((PROJECT / rommap.MAP_PATH).read_text())["ranges"]}
        cls.functions = json.loads((PROJECT / rommap.CATALOGUE_PATH).read_text())["identity"]["functions"]
        cls.pictures = t4_windows(cls.original, 0)
        cls.masks = t4_windows(cls.original, 8)

    def replay(self, document: dict) -> dict:
        return assetformat.replay(self.original, document, self.functions, self.rows, rommap.immediate_at,
                                  rommap.EXPECTED_SHA256, {row["id"]: row for row in self.rules["tables"]},
                                  owner_words(self.original, self.rules), map_base_rows())

    def mutated(self, offset: int, value: bytes) -> bytes:
        data = bytearray(self.original)
        data[offset:offset + len(value)] = value
        return bytes(data)

    def typed(self, data: bytes, fmt: str, interval: tuple[int, int]) -> list[Span]:
        spans = assetformat.PARSERS[fmt](data, (interval,), {"context": ()})
        return assetformat.check_tiling(data, spans, [interval])

    def test_replay_closes_every_t4_window(self) -> None:
        closed = self.replay(self.document)
        t4 = {interval: wid for interval, wid in closed.items() if wid.startswith("W_t4")}
        self.assertEqual(t4, {**{p: "W_t4_hvq2" for p in self.pictures}, **{m: "W_t4_masks" for m in self.masks}})
        self.assertEqual(sum(b - a for a, b in self.pictures), 324640)
        self.assertEqual(sum(b - a for a, b in self.masks), 21504)
        self.assertEqual({self.rows[interval] for interval in self.pictures}, {("asset_hvqm2_video", "T4")})
        self.assertEqual({self.rows[interval] for interval in self.masks}, {("asset_raw_referenced", "T4")})

    def test_picture_decodes_every_stream_exactly(self) -> None:
        self.assertEqual(self.pictures[1], self.PICTURE)
        spans = self.typed(self.original, "hvq2_still", self.PICTURE)
        fields = [s.field for s in spans]
        self.assertEqual(fields.count("hvq2_chunk.length"), 16)
        self.assertEqual(struct.unpack_from(">I", self.original, self.PICTURE[0] + 0x20)[0], 13554)
        # Fixed chunks 12,388 + 214 + 106 bytes are exactly the 6,354 basis codes of the coded blocks.
        self.assertEqual(fields.count("hvq2_basis.code"), 6354)
        self.assertEqual(fields.count("hvq2_rawpel.value"), 0)
        blank = [p for p in self.pictures if self.original[p[0]:p[1]] == self.original[self.BLANK[0]:self.BLANK[1]]]
        self.assertEqual(len(blank), 9)
        self.assertNotIn("hvq2_basis.code", {s.field for s in self.typed(self.original, "hvq2_still", self.BLANK)})

    def test_picture_tamper_fails(self) -> None:
        p = self.PICTURE[0]
        cases = {
            "version": (p + 5, b"2"),
            "file_size": (p + 0x10, struct.pack(">I", 18508)),
            "h_sampling": (p + 0x14, b"\x01"),
            "v_sampling": (p + 0x15, b"\x03"),
            "width": (p + 0x16, struct.pack(">H", 324)),
            "nest_y": (p + 0x1C, struct.pack(">H", 23)),
            "dc_shift": (p + 0x1F, b"\x09"),
            "work_units": (p + 0x20, struct.pack(">I", 13555)),
            "chunk offset": (p + 0x28, struct.pack(">I", 0x4A0)),
            "chunk length": (p + 0x49C, struct.pack(">I", 117)),
            "consumed bit": (p + 0x68, bytes([self.original[p + 0x68] ^ 0x80])),
            "unread bit": (p + 0x3F6, b"\x41"),   # mode_luma uses only the top 2 bits of its last byte
            "word fill": (p + 0x3F7, b"\x01"),
            "fixed fill": (p + 0x46FC + 4 + 214, b"\x01"),
            "tail fill": (p + 18504, b"\x01"),
            "blank picture gains a mode": (self.BLANK[0] + 0x69, b"\x80"),
        }
        for name, (offset, value) in cases.items():
            interval = self.BLANK if offset >= self.BLANK[0] else self.PICTURE
            with self.subTest(name), self.assertRaises(AssetFormatError):
                self.typed(self.mutated(offset, value), "hvq2_still", interval)

    def test_mask_tamper_fails(self) -> None:
        m = self.MASK[0]
        self.assertEqual(self.masks[1], self.MASK)
        spans = self.typed(self.original, "rle_mask", self.MASK)
        self.assertEqual(spans[-1], Span(m + 16 + 151, self.MASK[1], PAD, m + 16 + 151))
        cases = {
            "pixels_per_byte": (m + 3, b"\x04"),
            "data_offset": (m + 7, b"\x14"),
            "width": (m + 8, struct.pack(">H", 321)),
            "rle_bytes": (m + 12, struct.pack(">I", 152)),
            "run length": (m + 16, b"\x7e"),
            "end code": (m + 16 + 150, b"\x01"),
            "fill": (self.MASK[1] - 1, b"\x01"),
        }
        for name, (offset, value) in cases.items():
            with self.subTest(name), self.assertRaises(AssetFormatError):
                self.typed(self.mutated(offset, value), "rle_mask", self.MASK)

    def test_t4_record_tamper_fails_closed(self) -> None:
        def witness(document: dict, wid: str) -> dict:
            return next(w for w in document["witnesses"] if w["id"] == wid)

        def reads(document: dict, fmt: str, symbol: str) -> list:
            return next(c for c in document["formats"][fmt]["consumers"] if c["symbol"] == symbol)["reads"]

        def move(document: dict, wid: str, delta: int) -> None:
            anchor = witness(document, wid)["anchors"][0]
            anchor.update(site=anchor["site"] + delta)

        cases = {
            "picture anchor on the mask word": lambda d: move(d, "W_t4_hvq2", 8),
            "anchor off its pair": lambda d: move(d, "W_t4_masks", 4),
            "anchor table": lambda d: witness(d, "W_t4_hvq2")["anchors"][0].update(table="T3"),
            "start anchors dropped": lambda d: witness(d, "W_t4_hvq2").update(
                anchors=[a for a in witness(d, "W_t4_hvq2")["anchors"] if a["edge"] == "end"]),
            "masks as pictures": lambda d: witness(d, "W_t4_masks").update(format="hvq2_still"),
            "pictures as masks": lambda d: witness(d, "W_t4_hvq2").update(format="rle_mask"),
            "params": lambda d: witness(d, "W_t4_hvq2").update(params={"strict": True}),
            "context": lambda d: witness(d, "W_t4_hvq2").update(context=[witness(d, "W_t4_masks")["ranges"][0]]),
            "ref": lambda d: witness(d, "W_t4_hvq2")["ranges"][0].update(ref="T3"),
            "range hash": lambda d: witness(d, "W_t4_masks")["ranges"][3].update(sha256="0" * 64),
            "count": lambda d: witness(d, "W_t4_hvq2").update(span_count=witness(d, "W_t4_hvq2")["span_count"] - 1),
            "digest": lambda d: witness(d, "W_t4_masks").update(span_digest="0" * 64),
            "basis read width": lambda d: reads(d, "hvq2_still", "func_80132E00")[0].update(width=4),
            "header read offset": lambda d: reads(d, "hvq2_still", "func_8013591C")[0].update(offset=0x15),
            "mask read struct": lambda d: reads(d, "rle_mask", "func_80069A68")[0].update(struct="rle_code"),
            "unexercised access": lambda d: reads(d, "hvq2_still", "func_80132E00").append(
                {**reads(d, "hvq2_still", "func_80132E00")[0], "struct": "absent_struct"}),
        }
        for name, change in cases.items():
            document = copy.deepcopy(self.document)
            change(document)
            with self.subTest(name), self.assertRaises((AssetFormatError, rommap.MapError)):
                self.replay(document)


@unittest.skipUnless(PRIVATE_ROM.is_file(), "Private copied original required")
class OwnerWordTests(unittest.TestCase):
    """rommap.owner_words re-derives exactly the generated rows of the walked rules."""
    PALETTE_WORD = 0xE55730  # S_sprite_palette word with offset 0: the N_palette_e7be70 TLUT
    TLUT = (0xE7BE70, 0xE7BE90)

    @classmethod
    def setUpClass(cls) -> None:
        cls.original = PRIVATE_ROM.read_bytes()
        cls.rules = json.loads((PROJECT / rommap.RULES_PATH).read_text())
        cls.evidence = rommap.load_evidence(cls.original, cls.rules)
        cls.words = rommap.owner_words(cls.original, cls.rules, cls.evidence)
        cls.map_rows = json.loads((PROJECT / rommap.MAP_PATH).read_text())["ranges"]
        cls.rows = {(r[0], r[1]): (r[2], r[3]) for r in cls.map_rows}
        cls.functions = json.loads((PROJECT / rommap.CATALOGUE_PATH).read_text())["identity"]["functions"]

    def rows_of(self, ref: str, cls: str | None = None) -> set:
        """Generated rows of a walked rule, before witness sub-range splits."""
        return {(a, b) for (a, b), (c, r) in map_base_rows().items() if r == ref and (cls is None or c == cls)}

    def test_walks_reproduce_the_generated_rows(self) -> None:
        self.assertEqual(set(self.words["T5"].values()), self.rows_of("T5", "asset_demo_input"))
        self.assertEqual(len(self.words["T5"]), 6)
        iff = self.words["IFF_efat"]
        self.assertEqual(set(iff.values()), self.rows_of("IFF_efat", "asset_iff_payload"))
        self.assertEqual(len(iff), 3085)
        self.assertEqual({self.original[site:site + 4] for site in iff}, {b"SHRF", b"EGRF"})
        palette = self.words["S_sprite_palette"]
        self.assertEqual(len(palette), 757)
        # Every S_sprite_palette row, plus the one word whose TLUT is the code-based N_palette_e7be70 row.
        self.assertEqual(set(palette.values()), self.rows_of("S_sprite_palette") | {self.TLUT})
        self.assertEqual(palette[self.PALETTE_WORD], self.TLUT)
        self.assertEqual(self.rows[self.TLUT], ("asset_raw_code_based", "N_palette_e7be70"))

    def test_walks_reject_changed_structure(self) -> None:
        packed = min(self.words["IFF_efat"])
        directory = rommap.immediate_at(self.original, self.rules["sprite_palettes"][0]["directory_site"])
        cases = {
            "packed chunk length": (packed + 4, struct.pack(">I", struct.unpack_from(">I", self.original, packed + 4)[0] + 2)),
            "packed chunk stream": (packed + 12, b"Yay1"),
            "palette word segment": (self.PALETTE_WORD, b"\x05"),
            "palette table segment": (directory + next(r for r in range(0, 405 * 16, 16) if self.original[directory + r] & 31) + 4, b"\x06"),
            "demo capacity": (0x12722C, struct.pack(">I", 0x2804)),
        }
        for name, (offset, value) in cases.items():
            data = bytearray(self.original)
            data[offset:offset + len(value)] = value
            with self.subTest(name), self.assertRaises(rommap.MapError):
                rommap.owner_words(bytes(data), self.rules, self.evidence)

    def test_owner_word_anchor_replays_on_the_real_document(self) -> None:
        document = rommap.load_evidence(self.original, self.rules)[assetformat.INPUT]
        tables = {row["id"]: row for row in self.rules["tables"]}

        def replay(*anchors: dict, words=self.words) -> dict:
            changed = copy.deepcopy(document)
            witness = next(w for w in changed["witnesses"] if w["ranges"][0]["start"] == self.TLUT[0])
            witness["anchors"] = [{"kind": "owner_word", "site": self.PALETTE_WORD, "rule": "S_sprite_palette",
                                   "target": "range", "index": 0, "edge": "start", **anchor} for anchor in anchors]
            return assetformat.replay(self.original, changed, self.functions, self.rows, rommap.immediate_at,
                                      rommap.EXPECTED_SHA256, tables, words, map_base_rows())

        # The code-based TLUT witness closes the same row with the palette word instead of its immediate.
        self.assertEqual(replay({})[self.TLUT], "codebased_tlut_e7be70")
        self.assertEqual(replay({}, {"edge": "end"})[self.TLUT], "codebased_tlut_e7be70")
        for name, (anchors, words) in {"next palette word": (({"site": self.PALETTE_WORD + 4},), self.words),
                                       "other rule": (({"rule": "T5"},), self.words),
                                       "chain word": (({"site": 0x11D8BC, "rule": "T5"},), self.words),
                                       "end anchor only": (({"edge": "end"},), self.words),
                                       "no owner words": (({},), None)}.items():
            with self.subTest(name), self.assertRaises(AssetFormatError):
                replay(*anchors, words=words)



@unittest.skipUnless(PRIVATE_ROM.is_file(), "Private copied original required")
class SpriteModelGbiWitnessTests(unittest.TestCase):
    """opus-07: sprite directory and palettes, T3 model-group windows, GFX/WINDOW static display lists."""
    BANK = (0xE53DB0, 0xE7BE70)          # N_e53db0_block: the segment-5 sprite directory
    GFX = (0x157460, 0x1575C0)           # I_raw_157460: "GFX static", segment 1
    WINDOW = (0x1575C0, 0x157840)        # I_raw_1575c0: "WINDOW static", segment 5
    WINDOW_TEXTURE = (0x157840, 0x157B80)  # I_raw_157840: segment 6 of the WINDOW lists; stays open
    T3_TABLE = 0x10F53C                  # main_14400 D_8013C4FC, 17 x 36-byte scene records
    FORMATS = ("sprite_bank", "sprite_tlut16", "model_groups", "gbi_segment")

    @classmethod
    def setUpClass(cls) -> None:
        cls.original = PRIVATE_ROM.read_bytes()
        cls.rules = json.loads((PROJECT / rommap.RULES_PATH).read_text())
        cls.document = rommap.load_evidence(cls.original, cls.rules)[assetformat.INPUT]
        cls.rows = {(r[0], r[1]): (r[2], r[3]) for r in json.loads((PROJECT / rommap.MAP_PATH).read_text())["ranges"]}
        cls.functions = json.loads((PROJECT / rommap.CATALOGUE_PATH).read_text())["identity"]["functions"]
        cls.words = owner_words(cls.original, cls.rules)
        cls.witness = {w["id"]: w for w in cls.document["witnesses"]}
        cls.layer = {**cls.document, "formats": {k: cls.document["formats"][k] for k in cls.FORMATS},
                     "witnesses": [w for w in cls.document["witnesses"] if w["format"] in cls.FORMATS]}
        cls.windows = [struct.unpack_from(">II", cls.original, cls.T3_TABLE + 36 * k + 0x14) for k in range(17)]

    def u32(self, at: int) -> int:
        return struct.unpack_from(">I", self.original, at)[0]

    def patched(self, at: int, value: int, width: int = 1) -> bytes:
        data = bytearray(self.original)
        data[at:at + width] = value.to_bytes(width, "big")
        return bytes(data)

    def typed(self, wid: str, original: bytes | None = None, **params) -> list[Span]:
        data = original or self.original
        w = self.witness[wid]
        ranges = [(r["start"], r["end"]) for r in w["ranges"]]
        context = tuple((c["start"], c["end"]) for c in w["context"])
        spans = assetformat.PARSERS[w["format"]](data, tuple(ranges), {**w["params"], **params, "context": context})
        return assetformat.check_tiling(data, spans, ranges)

    def replay(self, document: dict, words: dict | None = None) -> dict:
        return assetformat.replay(self.original, document, self.functions, self.rows, rommap.immediate_at,
                                  rommap.EXPECTED_SHA256, {row["id"]: row for row in self.rules["tables"]},
                                  self.words if words is None else words, map_base_rows())

    def test_rows_are_witnessed_and_the_window_texture_stays_open(self) -> None:
        closed = self.replay(self.document)
        palettes = {k for k, (_, ref) in self.rows.items() if ref == "S_sprite_palette"}
        t3_raw = {k for k, (cls, ref) in self.rows.items() if ref == "T3" and cls == "asset_raw_referenced"}
        expected = {self.BANK: "sprite_bank_e53db0", self.GFX: "gbi_gfx_static_157460",
                    self.WINDOW: "gbi_window_static_1575c0", **{p: "sprite_palettes_s6" for p in palettes},
                    **{w: "model_groups_t3" for w in self.windows}}
        self.assertEqual({k: v for k, v in closed.items() if v in set(expected.values())}, expected)
        self.assertEqual((len(palettes), set(self.windows)), (756, t3_raw))
        self.assertEqual(sum(b - a for a, b in expected), 164032 + 24192 + 36992 + 352 + 640)
        self.assertEqual(self.rows[self.BANK], ("asset_raw_referenced", "N_e53db0_block"))
        self.assertEqual(self.rows[self.WINDOW_TEXTURE], ("asset_raw_referenced", "I_raw_157840"))
        self.assertNotIn(self.WINDOW_TEXTURE, closed)
        rom_map = rommap.load(PROJECT, self.original)
        self.assertTrue(frozenset(expected) <= rom_map.format_witnessed)
        result = rommap.account(rom_map, self.original, PROJECT, [], self.rules)
        statuses = {(r["start"], r["end"]): r["status"] for r in result["range_statuses"]}
        self.assertEqual({statuses[k] for k in expected}, {"classified_binary"})
        self.assertEqual(statuses[self.WINDOW_TEXTURE], "located")

    def test_sprite_bank_inventory(self) -> None:
        fields = [s.field for s in self.typed("sprite_bank_e53db0")]
        self.assertEqual([fields.count(f) for f in ("sprite_header.flags", "sprite_frame.data", "sprite_frame.step",
                                                     "sprite_palette_ref.word", PAD)], [405, 12875, 12875, 757, 2])
        # Every palette word of the bank is an owner word: 756 palette rows and the code-based TLUT.
        self.assertEqual(len(self.words["S_sprite_palette"]), fields.count("sprite_palette_ref.word"))

    def test_frame_step_is_read_through_the_pose_table(self) -> None:
        """Frame +0xA is Pose.step_0A: func_80074500 copies the header (0x10) and count*12 frame bytes, then
        func_8007A680 reads desc_4C->table_0C[frame_3F].step_0A with lb at 0x4DAA4 and 0x4DC14."""
        self.assertEqual([self.u32(s) for s in (0x47720, 0x47724, 0x47734, 0x47730, 0x4DAA4, 0x4DC14)],
                         [0x0C01AABC, 0x24060010, 0x90420008, 0x8C45000C, 0x8051000A, 0x8051000A])
        reads = {(c["symbol"], r["site"]) for c in self.layer["formats"]["sprite_bank"]["consumers"] for r in c["reads"]
                 if r["struct"] == "sprite_frame" and r["offset"] == 10}
        self.assertEqual(reads, {("func_8007A680", 0x4DAA4), ("func_8007A680", 0x4DC14)})

    def test_sprite_bank_tamper_fails(self) -> None:
        b = self.BANK[0]
        h = b + 16                                          # sprite 1
        frame, word = b + (self.u32(h + 12) & 0xFFFFFF), b + (self.u32(h + 4) & 0xFFFFFF)
        pad = next(s for s in self.typed("sprite_bank_e53db0") if s.field == PAD and s.end != self.BANK[1])
        cases = {
            "null sprite": self.patched(b + 8, 1),
            "header fill": self.patched(h + 1, 1),
            "frame count": self.patched(h + 8, self.original[h + 8] + 1),
            "palette count": self.patched(h, self.original[h] + 1),
            "frames segment": self.patched(h + 12, 6),
            "frame width": self.patched(frame + 6, self.original[frame + 6] + 2),
            "frame stream": self.patched(frame, self.u32(frame) + 4, 4),
            "frame packed size": self.patched(frame + 4, 16, 2),
            "frame unread +0xB": self.patched(frame + 11, 1),
            "palette word segment": self.patched(word, 5),
            "array fill": self.patched(pad.start, 1),
            "tail fill": self.patched(self.BANK[1] - 1, 1),
            "id bound": self.patched(0x44D74, 0x2CA20194, 4),
            "palette mask": self.patched(0x44DAC, 0x3063000F, 4),
            "frame transfer size": self.patched(0x44FDC, 0x2406000D, 4),
            "segment-6 base": self.patched(0x454AC, 0x2442BE80, 4),
        }
        for name, data in cases.items():
            with self.subTest(name), self.assertRaises(AssetFormatError):
                self.typed("sprite_bank_e53db0", data)

    def test_palette_tamper_fails(self) -> None:
        with self.assertRaises(AssetFormatError):   # a LoadTLUT of 15 entries
            self.typed("sprite_palettes_s6", self.patched(0x434B0, 0x354A8000, 4))
        document = copy.deepcopy(self.layer)
        next(w for w in document["witnesses"] if w["id"] == "sprite_palettes_s6")["ranges"][0]["end"] += 4
        with self.assertRaises(AssetFormatError):
            self.replay(document)

    def test_model_groups_tamper_fails(self) -> None:
        w, end = self.windows[0]
        group = next(p for p in range(w + 12, end, 4) if self.u32(p))
        record = self.T3_TABLE
        cases = {
            # (count + 1 would read the 4-byte zero fill as a null group, which func_80067860 skips)
            "group count past the window": self.patched(w + 7, self.original[w + 7] + 2),
            "groups pointer": self.patched(w + 3, 0x10),
            "reserved word": self.patched(w + 11, 1),
            "group segment": self.patched(group, 5),
            "list past the model": self.patched(group, 0x06FFFFF0, 4),
            "window fill": self.patched(end - 1, 1),
            "decoded offset": self.patched(0x343C4, 0x24430890, 4),
            "record bound": self.patched(0x3A694, 0x2C420010, 4),
            "record window": self.patched(record + 0x18, end + 4, 4),
            "record model": self.patched(record + 0x1C, self.u32(record + 0x1C) + 4, 4),
            "model stream": self.patched(self.u32(record + 0x1C) + 3, ord("1")),
        }
        for name, data in cases.items():
            with self.subTest(name), self.assertRaises(AssetFormatError):
                self.typed("model_groups_t3", data)

    def test_gbi_segment_tamper_fails(self) -> None:
        g = self.GFX[0]
        entries = self.witness["gbi_gfx_static_157460"]["params"]["entries"]
        cases = {
            "unknown opcode": (self.patched(g + 0x28, 0x01), {}),
            "pipesync operand": (self.patched(g + 0x27, 1), {}),
            "call outside the segment": (self.patched(g + 0xBC, 0x02), {}),
            "movemem index": (self.patched(g + 0x13, 0x0A), {}),
            "viewport padding": (self.patched(g + 0x7, 1), {}),
            "unreached list": (None, {"entries": [e for e in entries if e["offset"] != 0x140]}),
            "entry offset": (None, {"entries": [{**e, "offset": e["offset"] + 8} for e in entries]}),
            "entry segment": (self.patched(entries[0]["lui_site"] + 2, 0x05), {}),
        }
        for name, (data, params) in cases.items():
            with self.subTest(name), self.assertRaises(AssetFormatError):
                self.typed("gbi_gfx_static_157460", data, **params)

    def test_window_texture_rows_40_to_47_have_no_consumer(self) -> None:
        """I_raw_157840 stays open: the WINDOW lists load segment-6 bytes 0..0x280 (G_LOADBLOCK of 320
        16-bit texels for the 32x40 4-bit tiles) and two 16-entry TLUTs at 0x300 and 0x320; bytes
        0x280..0x300 (rows 40..47 of a 32-pixel 4-bit image) are loaded by no display list."""
        loaded, image = set(), None
        for span in self.typed("gbi_window_static_1575c0"):
            w0, w1 = struct.unpack_from(">II", self.original, span.start)
            if span.field.endswith(".dl_settimg") and (w1 >> 24) & 0xF == 6:
                image = (w1 & 0xFFFFFF, (1 << ((w0 >> 19) & 3)) >> 1)   # (offset, bytes per texel: 16b = 2)
            elif span.field.endswith(".dl_loadblock"):
                loaded.add((image[0], image[0] + image[1] * (((w1 >> 12) & 0xFFF) + 1)))
            elif span.field.endswith(".dl_loadtlut"):
                loaded.add((image[0], image[0] + 2 * (((w1 >> 14) & 0x3FF) + 1)))
        self.assertEqual(sorted(loaded), [(0, 0x280), (0x300, 0x320), (0x320, 0x340)])
        self.assertEqual(self.WINDOW_TEXTURE[1] - self.WINDOW_TEXTURE[0], 0x340)

    def test_record_tamper_fails_closed(self) -> None:
        def edit(change) -> dict:
            document = copy.deepcopy(self.layer)
            change({w["id"]: w for w in document["witnesses"]}, document["formats"])
            return document
        cases = {
            "bank start anchor": lambda w, f: w["sprite_bank_e53db0"]["anchors"].pop(0),
            "bank count": lambda w, f: w["sprite_bank_e53db0"]["params"].update(count=0x194),
            "frame size read offset": lambda w, f: f["sprite_bank"]["consumers"][1]["reads"][1].update(offset=5),
            "step read on +0xB": lambda w, f: f["sprite_bank"]["consumers"][5]["reads"][0].update(offset=11),
            "pose header transfer width": lambda w, f: f["sprite_bank"]["consumers"][4]["reads"][0].update(width=12),
            "palette start anchors": lambda w, f: w["sprite_palettes_s6"].update(anchors=[]),
            "palette end edge": lambda w, f: w["sprite_palettes_s6"]["anchors"][0].update(edge="end"),
            "palette transfer size": lambda w, f: f["sprite_tlut16"]["consumers"][0]["reads"][0].update(width=16),
            "t3 table anchor": lambda w, f: w["model_groups_t3"]["anchors"][0].update(table="T6"),
            "t3 read offset": lambda w, f: f["model_groups"]["consumers"][0]["reads"][1].update(offset=8),
            "palette next word": lambda w, f: w["sprite_palettes_s6"]["anchors"][0].update(
                site=w["sprite_palettes_s6"]["anchors"][0]["site"] + 4),
            "palette rule": lambda w, f: w["sprite_palettes_s6"]["anchors"][0].update(rule="T5"),
            "palette dropped": lambda w, f: w["sprite_palettes_s6"]["ranges"].pop(),
            "t3 digest": lambda w, f: w["model_groups_t3"].update(span_digest="0" * 64),
            "t3 context order": lambda w, f: w["model_groups_t3"]["context"].reverse(),
            "gbi read struct": lambda w, f: f["gbi_segment"]["consumers"][2]["reads"][2].update(struct="gfx_static"),
            "gbi entry dropped": lambda w, f: w["gbi_gfx_static_157460"]["params"]["entries"].pop(),
        }
        self.assertEqual(len(self.replay(self.layer)), 1 + 756 + 17 + 2)
        for name, change in cases.items():
            with self.subTest(name), self.assertRaises(AssetFormatError):
                self.replay(edit(change))
        with self.assertRaises(AssetFormatError):   # the palette witness needs the owner-word walk
            self.replay(self.layer, words={})


class SplitRowsTests(unittest.TestCase):
    """rommap.split_rows: witness-driven sub-range splits of generated rows (no ROM needed)."""
    rows = [[0, 16, "asset_raw_referenced", "R"], [16, 32, "asset_raw_code_based", "B"],
            [32, 48, "cpu_data", "main_14400"], [48, 64, "asset_yay0", "R"]]
    B = ("asset_raw_code_based", "B")

    def split(self, *pieces):
        return rommap.split_rows([list(row) for row in self.rows], pieces)

    def test_positive_split_keeps_every_remainder(self) -> None:
        rows, splits = self.split((18, 20, *self.B, "w1", (16, 32)), (24, 30, *self.B, "w2"))
        self.assertEqual(rows, [self.rows[0], [16, 18, *self.B], [18, 20, *self.B], [20, 24, *self.B],
                                [24, 30, *self.B], [30, 32, *self.B], *self.rows[2:]])
        self.assertEqual(splits, [{"parent": self.rows[1], "pieces": [[18, 20, *self.B, "w1"], [24, 30, *self.B, "w2"]]}])
        rommap.check_splits(rows, splits)
        self.assertEqual(rommap.unsplit_rows(rows, splits), self.rows)
        self.assertEqual(self.split(), (self.rows, []))
        # Edge-aligned sub-ranges leave remainders only where uncovered.
        rows, _ = self.split((16, 20, *self.B, "w1"), (28, 32, *self.B, "w1"))
        self.assertEqual(rows[1:4], [[16, 20, *self.B], [20, 28, *self.B], [28, 32, *self.B]])

    def test_overlap_crossing_and_class_changes_fail_closed(self) -> None:
        cases = {
            "overlapping witnesses": [(18, 22, *self.B, "w1"), (20, 24, *self.B, "w2")],
            "duplicate": [(18, 22, *self.B, "w1"), (18, 22, *self.B, "w1")],
            "crossing a row boundary": [(12, 20, "asset_raw_referenced", "R", "w1")],
            "past the last row": [(60, 70, "asset_yay0", "R", "w1")],
            "empty": [(20, 20, *self.B, "w1")],
            "named parent is not the containing row": [(18, 20, *self.B, "w1", (0, 16))],
            "whole located row": [(16, 32, *self.B, "w1")],
            "parent not located": [(50, 56, "asset_yay0", "R", "w1")],
            "piece changes class": [(18, 20, "asset_raw_referenced", "B", "w1")],
            "piece changes ref": [(18, 20, "asset_raw_code_based", "R", "w1")],
            "unruled reclass": [(36, 40, "image_opaque", "VT_x", "v1")],
            "unknown class": [(36, 40, "no_such_class", "VT_x", "v1")],
        }
        for name, pieces in cases.items():
            with self.subTest(name), self.assertRaises(rommap.MapError):
                self.split(*pieces)

    def test_ruled_reclass_takes_its_own_class_and_ref(self) -> None:
        with mock.patch.dict(rommap.SPLIT_RECLASS, {"cpu_data": frozenset({"image_opaque"})}):
            rows, splits = self.split((32, 40, "image_opaque", "VT_x", "v1"))
            self.assertEqual(rows[2:4], [[32, 40, "image_opaque", "VT_x"], [40, 48, "cpu_data", "main_14400"]])
            whole, _ = self.split((32, 48, "image_opaque", "VT_y", "v2"))
            self.assertEqual(whole[2], [32, 48, "image_opaque", "VT_y"])
            with self.assertRaises(rommap.MapError):  # the ruling is per parent class
                self.split((18, 20, "image_opaque", "VT_x", "v1"))
        self.assertEqual(splits[0]["parent"], self.rows[2])

    def test_remainder_that_changes_class_fails_closed(self) -> None:
        rows, splits = self.split((18, 20, *self.B, "w1"))
        tampered = {
            "remainder class": lambda r, s: r[3].__setitem__(2, "asset_raw_referenced"),
            "remainder ref": lambda r, s: r[3].__setitem__(3, "R"),
            "piece class": lambda r, s: r[2].__setitem__(2, "asset_raw_referenced"),
            "extra boundary": lambda r, s: r.__setitem__(slice(3, 4), [[20, 26, *self.B], [26, 32, *self.B]]),
            "piece missing": lambda r, s: s[0]["pieces"].append([24, 26, *self.B, "w9"]),
            "parent untiled": lambda r, s: r.__setitem__(3, [20, 30, *self.B]),
            "malformed record": lambda r, s: s[0].update(extra=1),
        }
        rommap.check_splits(rows, splits)
        for name, change in tampered.items():
            r, s = copy.deepcopy(rows), copy.deepcopy(splits)
            change(r, s)
            if name == "extra boundary":
                self.assertEqual([row[:2] for row in r[1:5]], [[16, 18], [18, 20], [20, 26], [26, 32]])
            with self.subTest(name), self.assertRaises(rommap.MapError):
                rommap.check_splits(r, s)

    def test_sol_f2_overlapping_split_parents_fail_closed(self) -> None:
        # SolReviewAssetStack F2 P3: rows [0,4),[4,8),[8,12),[12,16) of one class/ref, an outer parent
        # [0,16) and a nested parent [4,12) that both declare pieces [4,8)/w1 and [8,12)/w2.
        rows = [[a, a + 4, *self.B] for a in range(0, 16, 4)]
        pieces = [[4, 8, *self.B, "w1"], [8, 12, *self.B, "w2"]]
        outer = {"parent": [0, 16, *self.B], "pieces": copy.deepcopy(pieces)}
        rommap.check_splits(rows, [outer])
        self.assertEqual(rommap.unsplit_rows(rows, [outer]), [[0, 16, *self.B]])
        for name, splits in {"nested": [outer, {"parent": [4, 12, *self.B], "pieces": copy.deepcopy(pieces)}],
                             "duplicate": [outer, copy.deepcopy(outer)],
                             "overlapping": [{"parent": [0, 8, *self.B], "pieces": [pieces[0]]},
                                             {"parent": [4, 12, *self.B], "pieces": [pieces[1]]}]}.items():
            with self.subTest(name), self.assertRaisesRegex(rommap.MapError, "duplicate, overlapping or nested"):
                rommap.check_splits(rows, splits)
            with self.subTest(name + " inverse"), self.assertRaisesRegex(rommap.MapError, "duplicate, overlapping"):
                rommap.unsplit_rows(rows, splits)


class FlowEngineIdentityTests(unittest.TestCase):
    """The shared flow engine's bytes are part of the map's research-policy identity (SolReviewAssetStack
    F2 r2 finding 2): changing only mipsflow.py changes evidence_policy_sha256 and map_id."""

    def test_engine_only_change_changes_the_policy_identity(self) -> None:
        def document() -> dict:
            return rommap._document(bytes(range(16)), [[0, 16, "padding", "pad"]], rules_sha256="r",
                                    catalogue_id="c", catalogue={})
        before = document()
        with mock.patch.object(rommap.mipsflow, "__file__", __file__):  # any other bytes
            after = document()
        self.assertEqual(before["partition_sha256"], after["partition_sha256"])
        self.assertNotEqual(before["evidence_policy_sha256"], after["evidence_policy_sha256"])
        self.assertNotEqual(before["map_id"], after["map_id"])


class WithinReplayTests(unittest.TestCase):
    """assetformat.replay closes proper sub-ranges named with "within" (synthetic target)."""

    def setUp(self) -> None:
        self.code = struct.pack(">I", lw(2, 0, 4))  # lw v0, 0(a0): word.value at +0, 4 bytes
        self.data = self.code + bytes(4) + bytes(range(1, 25))
        self.function = {"symbol": "f", "rom_start": 0, "size": 4, "sha256": hashlib.sha256(self.code).hexdigest()}
        self.R = ("asset_raw_referenced", "R")
        self.base = {(8, 32): self.R}
        self.rows = {(8, 16): self.R, (16, 24): self.R, (24, 32): self.R}
        self.seen: list = []
        patcher = mock.patch.dict(assetformat.PARSERS, {"words": self.parse})
        patcher.start()
        self.addCleanup(patcher.stop)

    def parse(self, original, ranges, params):
        self.seen.append(dict(params))
        return [Span(a, a + 4, "word.value", a) for start, end in ranges for a in range(start, end, 4)]

    def record(self, start: int, end: int, within=(8, 32)) -> dict:
        row = {"start": start, "end": end, "class": self.R[0], "ref": self.R[1],
               "sha256": hashlib.sha256(self.data[start:end]).hexdigest()}
        return row | ({"within": {"start": within[0], "end": within[1]}} if within else {})

    def document(self, **witness) -> dict:
        anchors = [{"kind": "immediate", "site": 0, "function": self.function, "target": "within", "index": i,
                    "edge": "start"} for i in (0, 1)]
        record = {"id": "w", "format": "words", "ranges": [self.record(16, 24), self.record(24, 32)],
                  "context": [self.record(8, 32, within=None)], "anchors": anchors, "params": {},
                  "span_count": 4, "span_digest": assetformat.span_digest(
                      [Span(a, a + 4, "word.value", a) for a in range(16, 32, 4)]), "note": ""}
        record.update(witness)
        consumer = {**self.function, "image_id": "main_14400", "source": "src/f.c",
                    "reads": [{"site": 0, "word": f"{lw(2, 0, 4):08X}", "struct": "word", "offset": 0, "width": 4}]}
        return {"schema": assetformat.SCHEMA, "target_sha256": "T",
                "formats": {"words": {"consumers": [consumer], "note": ""}}, "witnesses": [record]}

    def replay(self, document: dict) -> dict:
        return assetformat.replay(self.data, document, [self.function], self.rows, lambda original, site: 8, "T",
                                  None, None, self.base)

    def test_sub_ranges_close_with_parent_anchors_and_base_context(self) -> None:
        self.assertEqual(self.replay(self.document()), {(16, 24): "w", (24, 32): "w"})
        self.assertEqual(self.seen[-1]["within"], ((8, 32), (8, 32)))
        self.assertEqual(self.seen[-1]["context"], ((8, 32),))

    def test_bad_sub_ranges_fail(self) -> None:
        cases = {
            "parent is not a pre-split row": {"ranges": [self.record(16, 24, (8, 24)), self.record(24, 32)]},
            "range is not a split row": {"ranges": [self.record(16, 20), self.record(24, 32)]},
            "parent of another class": {"ranges": [self.record(16, 24) | {"class": "asset_raw_code_based"},
                                                   self.record(24, 32)]},
            "one sub-range unanchored": {"anchors": self.document()["witnesses"][0]["anchors"][:1]},
            "within anchor on a whole row": {"ranges": [self.record(8, 16, within=None), self.record(24, 32)]},
            "within injected in params": {"params": {"within": ((8, 32),)}},
            "context row unknown": {"context": [self.record(8, 24, within=None)]},
            "within on a context": {"context": [self.record(8, 16)]},
        }
        for name, change in cases.items():
            with self.subTest(name), self.assertRaises(AssetFormatError):
                self.replay(self.document(**change))


@unittest.skipUnless(PRIVATE_ROM.is_file(), "Private copied original required")
class SegmentTwoBankTests(unittest.TestCase):
    """opus-10: the segment-2 game data bank B_194fc0 closes as four reviewed sub-ranges; the table-a
    directory and the two 21-record blocks (no consumer, no consumer-pinned struct) stay located."""
    PARENT = (0x194FC0, 0x1C6CB0)
    PIECES = [(0x194FC0, 0x1987B4), (0x198800, 0x1BAED8), (0x1BAF2C, 0x1BFE68), (0x1BFEBC, 0x1C6CB0)]
    OPEN = [(0x1987B4, 0x198800), (0x1BAED8, 0x1BAF2C), (0x1BFE68, 0x1BFEBC)]
    ROW = ("asset_raw_code_based", "B_194fc0")

    @classmethod
    def setUpClass(cls) -> None:
        cls.original = PRIVATE_ROM.read_bytes()
        cls.rules = json.loads((PROJECT / rommap.RULES_PATH).read_text())
        cls.document = rommap.load_evidence(cls.original, cls.rules)[assetformat.INPUT]
        cls.map = json.loads((PROJECT / rommap.MAP_PATH).read_text())
        cls.functions = json.loads((PROJECT / rommap.CATALOGUE_PATH).read_text())["identity"]["functions"]
        cls.catalogue = {(f["symbol"], f["rom_start"], f["size"]): f["sha256"] for f in cls.functions}
        cls.vrams = {(f["symbol"], f["rom_start"], f["size"]): f["vram_start"] for f in cls.functions}
        cls.witness = next(w for w in cls.document["witnesses"] if w["format"] == "seg2_game_data")
        cls.consumers = cls.document["formats"]["seg2_game_data"]["consumers"]

    def parse(self, original: bytes | None = None, ranges=None, **change) -> list:
        data = original or self.original
        ranges = tuple(ranges or self.PIECES)
        params = {**self.witness["params"], "context": tuple((c["start"], c["end"]) for c in self.witness["context"]),
                  "within": tuple(self.PARENT for _ in ranges), **change}
        return assetformat.check_tiling(data, assetformat.PARSERS["seg2_game_data"](data, ranges, params), ranges)

    def patched(self, at: int, value: int, width: int = 1) -> bytes:
        data = bytearray(self.original)
        data[at:at + width] = value.to_bytes(width, "big")
        return bytes(data)

    def test_sub_ranges_close_and_the_open_objects_stay_located(self) -> None:
        self.assertEqual([[r["start"], r["end"]] for r in self.witness["ranges"]], [list(p) for p in self.PIECES])
        self.assertTrue(all(r["within"] == {"start": self.PARENT[0], "end": self.PARENT[1]} for r in self.witness["ranges"]))
        split = next(s for s in self.map["splits"] if s["parent"][:2] == list(self.PARENT))
        self.assertEqual(split["parent"][2:], list(self.ROW))
        self.assertEqual([p[:2] for p in split["pieces"]], [list(p) for p in self.PIECES])
        rows = {(r[0], r[1]): (r[2], r[3]) for r in self.map["ranges"]}
        for interval in self.PIECES + self.OPEN:
            self.assertEqual(rows[interval], self.ROW)
        self.assertNotIn(self.PARENT, rows)
        self.assertEqual(map_base_rows()[self.PARENT], self.ROW)
        rom_map = rommap.load(PROJECT, self.original)
        self.assertTrue(set(self.PIECES) <= rom_map.format_witnessed)
        self.assertFalse(set(self.OPEN) & rom_map.format_witnessed)
        statuses = {(r["start"], r["end"]): r["status"] for r in
                    rommap.account(rom_map, self.original, PROJECT, [], self.rules)["range_statuses"]}
        self.assertEqual({statuses[p] for p in self.PIECES}, {"classified_binary"})
        self.assertEqual({statuses[p] for p in self.OPEN}, {"located"})
        self.assertEqual(sum(b - a for a, b in self.PIECES), 203772)
        self.assertEqual(sum(b - a for a, b in self.OPEN), 244)

    def test_every_byte_is_typed_and_every_pinned_access_agrees(self) -> None:
        ordered = self.parse()
        self.assertEqual((len(ordered), assetformat.span_digest(ordered)),
                         (self.witness["span_count"], self.witness["span_digest"]))
        matched = assetformat.check_reads(self.original, self.consumers, ordered, self.catalogue, self.vrams)
        pinned = {(r["site"], r["struct"]) for c in self.consumers for r in c["reads"]}
        self.assertEqual(matched, pinned)
        self.assertEqual(len(pinned), 132)
        instances: dict[str, set] = {}
        for s in ordered:
            if s.field != PAD:
                instances.setdefault(s.field.split(".")[0], set()).add(s.base)
        self.assertEqual(set(instances), {r["struct"] for c in self.consumers for r in c["reads"]})
        counts = {name: len(bases) for name, bases in instances.items()}
        self.assertEqual({k: counts[k] for k in ("seg2_level", "seg2_floor_header", "seg2_floor_record", "seg2_kind_stats",
                                                 "seg2_kind_attrs", "seg2_mlist_entry", "seg2_ilist_entry",
                                                 "seg2_tlist_entry", "seg2_wlist_entry")},
                         {"seg2_level": 693, "seg2_floor_header": 557, "seg2_floor_record": 526, "seg2_kind_stats": 260,
                          "seg2_kind_attrs": 260, "seg2_mlist_entry": 6986, "seg2_ilist_entry": 4884,
                          "seg2_tlist_entry": 8690, "seg2_wlist_entry": 33729})
        # The layout-only seg2_level table 0x1290..0x15A8 is typed like the consumer tables.
        layout_only = {s.base for s in ordered if s.field == "seg2_level.field_00" and 0x196250 <= s.base < 0x196568}
        self.assertEqual(len(layout_only), 99)
        for start, end in self.OPEN:
            self.assertFalse(any(s.start < end and start < s.end for s in ordered))

    def test_open_objects_cannot_be_typed(self) -> None:
        p0, p1, p2, p3 = self.PIECES
        for name, ranges in {"directory": [(p0[0], p1[1]), p2, p3], "first block": [p0, (p1[0], p2[1]), p3],
                             "second block": [p0, p1, (p2[0], p3[1])]}.items():
            with self.subTest(name), self.assertRaisesRegex(AssetFormatError, "not alignment fill"):
                self.parse(ranges=ranges)

    def test_tampered_bank_and_pins_fail(self) -> None:
        base = self.PARENT[0]
        resources = 0x115B5C
        mlist = base + (struct.unpack_from(">I", self.original, base + 0x2FD0)[0] & 0xFFFFFF)  # first table-a list
        ilist = base + (struct.unpack_from(">I", self.original, base + 0x3F50)[0] & 0xFFFFFF)  # first table-b list
        cases = {
            "mlist weight ends only one walk": self.patched(mlist + 2, 0, 2),
            "mlist id is no kind": self.patched(mlist, 0x60),
            "mlist level beyond its kind": self.patched(mlist + 1, 0xFF),
            "ilist fill halfword": self.patched(ilist + 2, 1, 2),
            "ilist wlist outside segment 2": self.patched(ilist + 4, 3),
            "kind range breaks the partition": self.patched(base + 0x25440 + 2, 4),
            "floor record weight index": self.patched(base + 0x26130 + 0x20, 5),
            "header row past the extents": self.patched(base + 0x15B0 + 4, 200),
            "weights11 rounding byte": self.patched(base + 0x260F4 + 11, 1),
            "alignment fill": self.patched(base + 0x2FCC, 1),
            "resource header segment": self.patched(resources, 3),
            "pinned table base": self.patched(0x17E3C, 0x24C60638, 4),
        }
        for name, data in cases.items():
            with self.subTest(name), self.assertRaises(AssetFormatError):
                self.parse(data)

    def test_witness_shape_is_required(self) -> None:
        with self.assertRaises(AssetFormatError):
            self.parse(within=None)
        with self.assertRaises(AssetFormatError):
            self.parse(within=((0x194FC0, 0x1C6CB0), (0x179560, 0x194FC0), (0x194FC0, 0x1C6CB0), (0x194FC0, 0x1C6CB0)))
        extents = copy.deepcopy(self.witness["params"]["extents"])
        extents["seg2_floor_record"][0][1] -= 1
        with self.assertRaises(AssetFormatError):
            self.parse(extents=extents)
        tables = copy.deepcopy(self.witness["params"]["tables"])
        next(t for t in tables if t["seg"] == 0x1290)["layout_only"] = False
        with self.assertRaises(AssetFormatError):
            self.parse(tables=tables)

    # r2 (SolReviewSeg2Bank F1). The ilist entry at ROM 0x198C08 is the only reference to the 10-byte wlist
    # at seg 0x38A4 (ROM 0x198864), which directly follows an object and ends at a 16-byte boundary, so
    # once unreferenced and zeroed its bytes are legal alignment fill. Retargeting the entry then isolates
    # the wlist-target rule from tiling.
    WLIST_SITE, WLIST_SEG, WLIST_BYTES, SHARED_SEG = 0x198C0C, 0x38A4, 10, 0x3860

    def retargeted(self, seg: int) -> bytes:
        start = self.PARENT[0] + self.WLIST_SEG
        self.assertEqual(struct.unpack_from(">I", self.original, self.WLIST_SITE)[0], 0x02000000 | self.WLIST_SEG)
        data = self.patched(self.WLIST_SITE, 0x02000000 | seg, 4)
        return data[:start] + bytes(self.WLIST_BYTES) + data[start + self.WLIST_BYTES:]

    def test_shared_wlist_is_parsed_once(self) -> None:
        sites = [s.start for s in self.parse() if s.field == "seg2_ilist_entry.wlist"
                 and struct.unpack_from(">I", self.original, s.start)[0] & 0xFFFFFF == self.SHARED_SEG]
        self.assertEqual(len(sites), 1)
        ordered = self.parse(self.retargeted(self.SHARED_SEG))
        shared = self.PARENT[0] + self.SHARED_SEG
        self.assertEqual(len([s for s in ordered if s.field == "seg2_wlist_entry.id" and s.base == shared]), 1)
        self.assertEqual(len(ordered), self.witness["span_count"] - self.WLIST_BYTES)

    def test_wlist_target_must_be_a_wlist(self) -> None:
        mlist = struct.unpack_from(">I", self.original, self.PARENT[0] + 0x2FD0)[0] & 0xFFFFFF
        for seg, message in ((0x23BE0, "is a seg2_kind_stats object"), (0x15B0, "is a seg2_floor_header object"),
                             (0x2FD0, "is a seg2_mlist_ref object"), (0x26130, "is a seg2_floor_record object"),
                             (0x0, "is a seg2_level object"), (mlist, "reached as mlist and wlist")):
            with self.subTest(seg=hex(seg)), self.assertRaisesRegex(AssetFormatError, message):
                self.parse(self.retargeted(seg))

@unittest.skipUnless(PRIVATE_ROM.is_file(), "Private copied original required")
class IffEfatPayloadWitnessTests(unittest.TestCase):
    """opus-06: non-Yay0 chunk payloads of the EFAT archive (WP9, asset_iff_payload)."""
    DIRECTORY = (0x1A56E60, 0x1A57CE8)          # "EFAT" header + 464 entries, ends at the first FORM
    FIRST_FORM = (0x1A57CE8, 0x1A58B99)         # EGRP(0xC4) TIMG(0xC) SHRF(0x23) EGRF x 8
    SOLE_GRAPH_FORM_FIRST = (0x1AB57DA, 0x1AB586A)   # EGRP of 8 groups, one EGRF: sole offset 0
    ZERO_DOMINATED = (0x1ACB2D6, 0x1ACB392)     # one of the 18 rows that stay located

    @classmethod
    def setUpClass(cls) -> None:
        cls.original = PRIVATE_ROM.read_bytes()
        cls.rules = json.loads((PROJECT / rommap.RULES_PATH).read_text())
        cls.document = rommap.load_evidence(cls.original, cls.rules)[assetformat.INPUT]
        cls.map_rows = json.loads((PROJECT / rommap.MAP_PATH).read_text())["ranges"]
        cls.rows = {(r[0], r[1]): (r[2], r[3]) for r in cls.map_rows}
        cls.functions = json.loads((PROJECT / rommap.CATALOGUE_PATH).read_text())["identity"]["functions"]
        cls.witness = next(w for w in cls.document["witnesses"] if w["id"].startswith("iff_efat_"))
        cls.ranges = tuple((r["start"], r["end"]) for r in cls.witness["ranges"])
        cls.payload_rows = sorted((r[0], r[1]) for r in cls.map_rows if r[2] == "asset_iff_payload")
        cls.words = rommap.owner_words(cls.original, cls.rules, rommap.load_evidence(cls.original, cls.rules))
    def patched(self, at: int, value: int, width: int = 1, original: bytes | None = None) -> bytes:
        data = bytearray(original or self.original)
        data[at:at + width] = value.to_bytes(width, "big")
        return bytes(data)
    def parse(self, original: bytes, ranges: tuple) -> list:
        spans = assetformat.PARSERS["iff_efat_payload"](original, ranges, {"context": (self.DIRECTORY,)})
        return assetformat.check_tiling(original, spans, ranges)

    def replay(self, document: dict, original: bytes | None = None) -> dict:
        return assetformat.replay(original or self.original, document, self.functions, self.rows,
                                  rommap.immediate_at, rommap.EXPECTED_SHA256,
                                  {row["id"]: row for row in self.rules["tables"]}, self.words, map_base_rows())

    def test_anchors_are_the_packed_chunk_tag_words_of_their_rows(self) -> None:
        iff = self.words["IFF_efat"]
        self.assertEqual(set(iff.values()), set(self.payload_rows))
        for anchor in self.witness["anchors"]:
            self.assertEqual(iff[anchor["site"]], self.ranges[anchor["index"]])
            self.assertIn(self.original[anchor["site"]:anchor["site"] + 4], (b"SHRF", b"EGRF"))
        self.assertEqual(sorted(a["index"] for a in self.witness["anchors"]), list(range(len(self.ranges))))

    def test_replay_closes_every_typed_payload_row_and_the_zero_dominated_rows_stay_located(self) -> None:
        self.assertEqual(len(self.payload_rows), 3085)
        self.assertEqual(self.rows[self.DIRECTORY], ("asset_iff_container_meta", "IFF_efat"))
        closed = {k: v for k, v in self.replay(self.document).items() if v == self.witness["id"]}
        self.assertEqual(set(closed), set(self.ranges))
        self.assertEqual((len(closed), sum(b - a for a, b in closed)), (3067, 149275))
        open_rows = sorted(set(self.payload_rows) - set(closed))
        self.assertEqual((len(open_rows), sum(b - a for a, b in open_rows)), (18, 3441))
        self.assertIn(self.ZERO_DOMINATED, open_rows)
        rom_map = rommap.load(PROJECT, self.original)
        self.assertTrue(frozenset(closed) <= rom_map.format_witnessed)
        result = rommap.account(rom_map, self.original, PROJECT, [], self.rules)
        statuses = {(r["start"], r["end"]): r["status"] for r in result["range_statuses"]}
        self.assertEqual(statuses[self.FIRST_FORM[0] + 12, 0x1A57DE0], "classified_binary")
        self.assertEqual(statuses[self.ZERO_DOMINATED], "located")

    def test_zero_dominated_group_tables_cannot_close(self) -> None:
        with self.assertRaisesRegex(AssetFormatError, "dominated by zero"):
            self.parse(self.original, (self.ZERO_DOMINATED,))
        document = copy.deepcopy(self.document)
        for w in document["witnesses"]:
            if w["id"] == self.witness["id"]:
                a, b = self.ZERO_DOMINATED
                w["ranges"].append({"start": a, "end": b, "class": "asset_iff_payload", "ref": "IFF_efat",
                                    "sha256": hashlib.sha256(self.original[a:b]).hexdigest()})
                w["anchors"].append({"kind": "owner_word", "site": b - 12, "rule": "IFF_efat", "target": "range",
                                     "index": len(w["ranges"]) - 1, "edge": "start"})
        with self.assertRaisesRegex(AssetFormatError, "dominated by zero"):
            self.replay(document)

    def test_first_payload_types_every_chunk_and_stops_at_the_yay0_stream(self) -> None:
        start = self.FIRST_FORM[0] + 12
        spans = self.parse(self.original, ((start, 0x1A57DE0),))
        fields = [s.field for s in spans]
        self.assertEqual(fields[:3], ["chunk.tag", "chunk.size", "egrp.count"])
        self.assertEqual(fields[-3:], ["chunk.tag", "chunk.size", "packed.unpacked_size"])
        self.assertEqual(self.original[0x1A57DE0:0x1A57DE4], b"Yay0")
        self.assertEqual(sum(f == "egrp_group.graph_offset" for f in fields), 8)
        self.assertIn("timg.count", fields)

    def test_sole_graph_offset_is_required_zero(self) -> None:
        spans = self.parse(self.original, (self.SOLE_GRAPH_FORM_FIRST,))
        sole = [s for s in spans if s.field == "egrp_group.sole_graph_offset"]
        self.assertEqual(len(sole), 8)
        with self.assertRaises(AssetFormatError):   # the only EGRF chunk is at offset 0
            self.parse(self.patched(sole[3].start, 0x1B9, 4), (self.SOLE_GRAPH_FORM_FIRST,))

    def test_chunk_egrp_timg_and_packed_tamper_fail(self) -> None:
        start, first_payload = self.FIRST_FORM[0], (self.FIRST_FORM[0] + 12, 0x1A57DE0)
        egrp = start + 12
        timg = 0x1A57DC0
        shrf = 0x1A57DD4
        cases = {
            "egrp size": self.patched(egrp + 4, 0xC8, 4),
            "egrp count": self.patched(egrp + 8, 7, 4),
            "graph offset not an EGRF chunk": self.patched(egrp + 12, 4, 4),
            "list count past the chunk": self.patched(egrp + 16, 0x100, 4),
            "timg key count": self.patched(timg + 16, 1, 4),
            "unpacked size differs from the Yay0 header": self.patched(shrf + 8, 0x15, 4),
            "base chunk without a derived grammar": self.patched(timg, 0x45484452, 4),   # EHDR
            "unknown chunk tag": self.patched(egrp, 0x45475251, 4),
        }
        for name, data in cases.items():
            with self.subTest(name), self.assertRaises(AssetFormatError):
                self.parse(data, (first_payload,))

    def test_form_walk_and_directory_tamper_fail(self) -> None:
        payload = (self.FIRST_FORM[0] + 12, 0x1A57DE0)
        cases = {
            "FORM type": self.patched(self.FIRST_FORM[0] + 8, 0x45534646, 4),        # ESFF is not compressed
            "directory count": self.patched(self.DIRECTORY[0] + 4, 465, 4),
            "directory offset": self.patched(self.DIRECTORY[0] + 8, 2, 4),
            "chunk escapes its FORM": self.patched(0x1A57DFF + 4, 0x10000, 4),       # first EGRF chunk size
        }
        for name, data in cases.items():
            with self.subTest(name), self.assertRaises(AssetFormatError):
                self.parse(data, (payload,))

    def test_shifted_or_unwalked_intervals_fail(self) -> None:
        a, b = self.FIRST_FORM[0] + 12, 0x1A57DE0
        for interval in ((a, b - 4), (a + 8, b), (a, b + 4)):
            with self.subTest(interval), self.assertRaises(AssetFormatError):
                self.parse(self.original, (interval,))
        document = copy.deepcopy(self.document)
        for w in document["witnesses"]:
            if w["id"] == self.witness["id"]:
                w["ranges"][0]["end"] -= 4
        with self.assertRaises(AssetFormatError):
            self.replay(document)

    def test_record_tamper_fails_closed(self) -> None:
        mutations = [
            lambda w, f: w.update(span_count=w["span_count"] + 1),
            lambda w, f: w.update(span_digest="0" * 64),
            lambda w, f: w.update(context=[]),
            lambda w, f: w["anchors"].pop(),
            lambda w, f: w["ranges"].pop(),
            lambda w, f: f["consumers"][0]["reads"][0].update(width=12),
            lambda w, f: f["consumers"][1]["reads"][1]["transfer_call"].update(site=0x61778),   # the other call
            lambda w, f: f["consumers"][3].update(sha256="0" * 64),
            lambda w, f: f["consumers"][3]["reads"][0].update(offset=4, word="8C420004"),   # graph_offset load
            lambda w, f: w["anchors"][5].update(site=w["anchors"][5]["site"] + 4),
            lambda w, f: w["anchors"][7].update(index=8),
        ]
        for index, mutate in enumerate(mutations):
            document = copy.deepcopy(self.document)
            witness = next(w for w in document["witnesses"] if w["id"] == self.witness["id"])
            mutate(witness, document["formats"]["iff_efat_payload"])
            with self.subTest(index), self.assertRaises(AssetFormatError):
                self.replay(document)


@unittest.skipUnless(PRIVATE_ROM.is_file(), "Private copied original required")
class T5SaveStreamWitnessTests(unittest.TestCase):
    """opus-06 r2: each T5 buffer (WP11, asset_demo_input) closes header..PtnerBkup, PStatus..end and
    the value pair. The PtnerBkup section (typed, zero-dominated) and the unread tail [end, 0x27F8),
    which has no pinned content writer, stay located."""
    STARTS = tuple(0x148460 + 0x2800 * i for i in range(6))
    ENDS = (0xACF, 0xB5B, 0xB69, 0xC4E, 0xB93, 0xC29)
    BACKUPS = (0x4C0, 0x50B, 0x4F9, 0x55E, 0x51B, 0x5C2)   # PtnerBkup tag; the section is 1,474 bytes
    COMMANDS = (46, 87, 112, 190, 116, 104)
    ROW = ("asset_demo_input", "T5")

    @classmethod
    def setUpClass(cls) -> None:
        cls.original = PRIVATE_ROM.read_bytes()
        cls.rules = json.loads((PROJECT / rommap.RULES_PATH).read_text())
        cls.document = rommap.load_evidence(cls.original, cls.rules)[assetformat.INPUT]
        cls.map = json.loads((PROJECT / rommap.MAP_PATH).read_text())
        cls.rows = {(r[0], r[1]): (r[2], r[3]) for r in cls.map["ranges"]}
        cls.functions = json.loads((PROJECT / rommap.CATALOGUE_PATH).read_text())["identity"]["functions"]
        cls.witness = next(w for w in cls.document["witnesses"] if w["id"] == "demo_t5_save_streams")
        cls.parents = tuple((s, s + 0x2800) for s in cls.STARTS)
        cls.heads = tuple((s, s + k) for s, k in zip(cls.STARTS, cls.BACKUPS))
        cls.backups = tuple((s + k, s + k + 1474) for s, k in zip(cls.STARTS, cls.BACKUPS))
        cls.statuses = tuple((s + k + 1474, s + e) for s, k, e in zip(cls.STARTS, cls.BACKUPS, cls.ENDS))
        cls.pairs = tuple((s + 0x27F8, s + 0x2800) for s in cls.STARTS)
        cls.tails = tuple((s + e, s + 0x27F8) for s, e in zip(cls.STARTS, cls.ENDS))
        cls.pieces = tuple(sorted(cls.heads + cls.statuses + cls.pairs))
        cls.open = tuple(sorted(cls.backups + cls.tails))

    def patched(self, at: int, value: int, width: int = 1, original: bytes | None = None) -> bytes:
        data = bytearray(original or self.original)
        data[at:at + width] = value.to_bytes(width, "big")
        return bytes(data)

    def parse(self, original: bytes, ranges: tuple | None = None, within: tuple | None = None) -> list:
        ranges = ranges or self.pieces
        within = within or tuple(next(p for p in self.parents if p[0] <= r[0] < p[1]) for r in ranges)
        spans = assetformat.PARSERS["t5_save_stream"](original, ranges, {"context": (), "within": within})
        return assetformat.check_tiling(original, spans, ranges)

    def replay(self, document: dict, original: bytes | None = None) -> dict:
        return assetformat.replay(original or self.original, document, self.functions, self.rows,
                                  rommap.immediate_at, rommap.EXPECTED_SHA256,
                                  {row["id"]: row for row in self.rules["tables"]},
                                  owner_words(original or self.original, self.rules), map_base_rows())

    def rechecksum(self, data: bytes, index: int) -> bytes:
        start, end = self.STARTS[index], self.ENDS[index]
        value = assetformat.T5_CHECKSUM_SEED
        for byte in data[start + 8:start + end]:
            value = assetformat._t5_mix(value) ^ byte
        return self.patched(start + 4, value, 4, data)

    def test_typed_pieces_close_and_the_backups_and_unread_tails_stay_located(self) -> None:
        self.assertEqual([(r["start"], r["end"]) for r in self.witness["ranges"]], list(self.pieces))
        for record in self.witness["ranges"]:
            parent = next(p for p in self.parents if p[0] <= record["start"] < p[1])
            self.assertEqual(record["within"], {"start": parent[0], "end": parent[1]})
        for parent, head, status, pair in zip(self.parents, self.heads, self.statuses, self.pairs):
            split = next(s for s in self.map["splits"] if s["parent"][:2] == list(parent))
            self.assertEqual(split["parent"][2:], list(self.ROW))
            self.assertEqual([p[:2] for p in split["pieces"]], [list(head), list(status), list(pair)])
            self.assertNotIn(parent, self.rows)
            self.assertEqual(map_base_rows()[parent], self.ROW)
        for interval in self.pieces + self.open:
            self.assertEqual(self.rows[interval], self.ROW)
        closed = self.replay(self.document)
        self.assertEqual({k for k, v in closed.items() if v == "demo_t5_save_streams"}, set(self.pieces))
        rom_map = rommap.load(PROJECT, self.original)
        self.assertTrue(set(self.pieces) <= rom_map.format_witnessed)
        self.assertFalse(set(self.open) & rom_map.format_witnessed)
        statuses = {(r["start"], r["end"]): r["status"] for r in
                    rommap.account(rom_map, self.original, PROJECT, [], self.rules)["range_statuses"]}
        self.assertEqual({statuses[p] for p in self.pieces}, {"classified_binary"})
        self.assertEqual({statuses[t] for t in self.open}, {"located"})
        self.assertEqual(sum(b - a for a, b in self.heads + self.statuses), 8977)
        self.assertEqual(sum(b - a for a, b in self.pairs), 48)
        self.assertEqual(sum(b - a for a, b in self.backups), 8844)
        self.assertEqual(sum(b - a for a, b in self.tails), 43571)

    def test_stream_parses_to_the_end_word_and_no_open_byte_is_claimed(self) -> None:
        for index, (start, end) in enumerate(zip(self.STARTS, self.ENDS)):
            spans: list = []
            stream = assetformat._T5Stream(self.original, start, end, spans)
            assetformat._t5_state(self.original, stream)
            self.assertEqual(assetformat._t5_commands(self.original, stream), self.COMMANDS[index])
            self.assertEqual(stream.pos, start + end)
        ordered = self.parse(self.original)
        self.assertEqual((len(ordered), assetformat.span_digest(ordered)),
                         (self.witness["span_count"], self.witness["span_digest"]))
        for start, end in self.open:
            self.assertFalse(any(s.start < end and start < s.end for s in ordered))
        for start, (backup, _) in zip(self.STARTS, self.backups):
            self.assertEqual(self.original[backup:backup + 2], b"\x21\xae")         # "PtnerBkup" tag
            self.assertEqual(assetformat.t5_pieces(self.original, start)[0], (start, backup))

    def test_the_tail_and_the_backup_cannot_close(self) -> None:
        h, b, s, t, p = self.heads[0], self.backups[0], self.statuses[0], self.tails[0], self.pairs[0]
        cases = {
            "whole buffer row": ((self.parents[0],), (None,)),
            "stream extended over the tail": ((h, (s[0], p[0]), p), None),
            "tail named as a piece": ((h, s, t, p), None),
            "header through the logical end": (((h[0], s[1]), p), None),
            "backup named as a piece": ((h, b, s, p), None),
            "a piece missing": ((h, s), None),
        }
        for name, (ranges, within) in cases.items():
            with self.subTest(name), self.assertRaises(AssetFormatError):
                self.parse(self.original, ranges, within)
        document = copy.deepcopy(self.document)
        witness = next(w for w in document["witnesses"] if w["id"] == self.witness["id"])
        a, b = self.tails[2]
        witness["ranges"].append({"start": a, "end": b, "class": self.ROW[0], "ref": self.ROW[1],
                                  "sha256": hashlib.sha256(self.original[a:b]).hexdigest(),
                                  "within": {"start": self.parents[2][0], "end": self.parents[2][1]}})
        witness["anchors"].append({"kind": "owner_word", "site": 0x11D8BC + 8, "rule": "T5", "target": "within",
                                   "index": len(witness["ranges"]) - 1, "edge": "start"})
        with self.assertRaises(AssetFormatError):
            self.replay(document)
        backup = self.backups[0]
        typed = assetformat._t5_walk(self.original, self.STARTS[0])[0]
        with self.assertRaisesRegex(AssetFormatError, "dominated by zero"):     # the backup alone
            assetformat.check_tiling(self.original, [sp for sp in typed if backup[0] <= sp.start < backup[1]], [backup])

    def test_tail_bytes_are_neither_read_nor_claimed(self) -> None:
        start, end = self.STARTS[0], self.ENDS[0]
        data = self.patched(start + end + 0x10, self.original[start + end + 0x10] ^ 0xFF)
        self.assertEqual(self.parse(data), self.parse(self.original))
        closed = self.replay(self.document, data)
        self.assertEqual({k for k, v in closed.items() if v == "demo_t5_save_streams"}, set(self.pieces))

    def test_header_checksum_and_value_pair_tamper_fail(self) -> None:
        start = self.STARTS[1]
        cases = {
            "checksum": self.patched(start + 4, 0, 4),
            "end word": self.patched(start, self.ENDS[1] + 1, 4),
            "end past the value pair": self.patched(start, 0x27FC, 4),
            "value check": self.patched(start + 0x27FC, 0, 4),
        }
        for name, data in cases.items():
            with self.subTest(name), self.assertRaises(AssetFormatError):
                self.parse(data)

    def test_section_record_and_command_tamper_fail(self) -> None:
        start, end = self.STARTS[0], self.ENDS[0]
        first_command = start + 0xA85
        cases = {
            "RecStat tag": self.patched(start + 8, 0x0787, 2),
            "PtnerBkup tag": self.patched(start + self.BACKUPS[0], 0x21AF, 2),
            "moment left to runtime state": self.patched(start + 0x11, self.original[start + 0x11] & ~2),
            "resumed record (Eye/Audio)": self.patched(start + 0x24, 1),
            "command without a class": self.patched(first_command, 0x36),
            "opcode at or above 0x3F": self.patched(first_command, 0x3F),
            "command overruns the end": self.patched(start + end - 1, 0x10),
        }
        for name, data in cases.items():
            with self.subTest(name), self.assertRaises(AssetFormatError):
                self.parse(self.rechecksum(data, 0), (self.heads[0], self.statuses[0], self.pairs[0]))

    def test_item_class_and_factory_tamper_fail(self) -> None:
        spans = self.parse(self.original, (self.heads[0], self.statuses[0], self.pairs[0]))
        item = next(s for s in spans if s.field == "item_slot.id" and 1 <= self.original[s.start] < 23)   # kind 1
        weapon_vtable = 0x8015D518 - assetformat.T5_VRAM
        cases = {
            "item id without a derived class": self.rechecksum(self.patched(item.start, 3), 0),
            "factory slot": self.patched(0x80157CD8 - assetformat.T5_VRAM + 4 * (self.original[item.start] - 1),
                                         0x80117319, 4),
            "class load method": self.patched(weapon_vtable + 0x2C, 0x800AF174, 4),
        }
        for name, data in cases.items():
            with self.subTest(name), self.assertRaises(AssetFormatError):
                self.parse(data)

    def test_section_tags_are_the_consumer_name_checksums(self) -> None:
        self.assertEqual(assetformat._t5_crc(b"RecStat"), 0x0786)
        self.assertEqual(assetformat._t5_crc(b"PStatus"), 0x4FED)
        self.assertEqual(assetformat._t5_crc(b"EventFlg"), 0xFEF6)

    def test_record_tamper_fails_closed(self) -> None:
        mutations = [
            lambda w, f: w.update(span_count=w["span_count"] + 1),
            lambda w, f: w.update(span_digest="0" * 64),
            lambda w, f: w["anchors"].pop(),
            lambda w, f: w["anchors"][2].update(site=w["anchors"][2]["site"] + 4),
            lambda w, f: w.update(params={"capacity": 0x2800}),
            lambda w, f: w["ranges"][0].pop("within"),
            lambda w, f: w["ranges"][1]["within"].update(start=self.parents[1][0]),
            lambda w, f: w["ranges"].pop(1),
            lambda w, f: f["consumers"][0]["reads"][0].update(width=8),
            lambda w, f: f["consumers"][0]["reads"][1].update(struct="rec_unread"),   # pin exercised by no span
        ]
        for index, mutate in enumerate(mutations):
            document = copy.deepcopy(self.document)
            witness = next(w for w in document["witnesses"] if w["id"] == self.witness["id"])
            mutate(witness, document["formats"]["t5_save_stream"])
            with self.subTest(index), self.assertRaises(AssetFormatError):
                self.replay(document)


if __name__ == "__main__":
    unittest.main(verbosity=2)
