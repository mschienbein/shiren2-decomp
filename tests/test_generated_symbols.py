"""H22 focused tests: native pinned g++ object/link proof, no full-ROM build.

The local-static fixture uses a synthetic catalogue and a frozen 124-byte golden
text image in a synthetic 32 MiB ROM, not a claim of game coverage. Fixed-target
ROM attestation is mocked for these fixtures; compiler, assembler, linker,
ELF/map, image hashes and absence scans remain real. Outputs remain in TMPDIR.
"""
from __future__ import annotations

import copy
from dataclasses import replace
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
from types import SimpleNamespace
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
import certification as cert
import generated_symbols as identities
import match
import progress
from evidence import validate_attributions, validate_matches
from rom import EXPECTED_SIZE, attest as real_rom_attest

PROFILE = "gxx281pm-gnu291-O2-unsigned"
SOURCE = "src/local_static.cpp"
OBJECT = "obj/source/src/local_static.o"
TEXT = bytes.fromhex(
    "27bdffe83c0480042484a000afbf00100c00c000000000008fbf001003e0000827bd0018"
    "3c0280048c42900027bdffe8afb000100080802d14400007afbf0014240200013c018004"
    "ac2290003c0480020c0363cc24845c008fbf00140200102d3c018004ac30a0008fb00010"
    "03e0000827bd001803e0000800000000"
)
IMAGE = {"resident": {"rom_start": 0x1000, "rom_end": 0x12340, "vram_start": 0x80025C00}}


def write_json(path: Path, value: object) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(value, indent=2) + "\n")


class GeneratedSymbolsTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls) -> None:
        private_tmp = Path(os.environ.get("TMPDIR", ""))
        if not private_tmp.is_absolute() or not private_tmp.is_dir():
            raise RuntimeError("Set a private absolute TMPDIR for these compiler tests")
        cls.work = Path(tempfile.mkdtemp(prefix="h22-generated-", dir=private_tmp))
        cls.profile = cert.tool_profile(ROOT)
        cls.graph = {OBJECT: {"source": SOURCE, "input": "source/" + SOURCE, "source_kind": "cpp", "image_id": "resident"}}
        cls.rows = [
            {"image_id": "resident", "symbol": "func_80025C00", "source": SOURCE, "rom_start": 0x1000,
             "vram_start": 0x80025C00, "size": 36, "generated": "__tcf_0"},
            {"image_id": "resident", "symbol": "use_static", "source": SOURCE, "rom_start": 0x1024,
             "vram_start": 0x80025C24, "size": 80},
            {"image_id": "resident", "symbol": "func_80025C74", "source": SOURCE, "rom_start": 0x1074,
             "vram_start": 0x80025C74, "size": 8, "generated": "_GLOBAL_.I.use_static"},
        ]
        cls.unit = {"image_id": "resident", "functions": [r["symbol"] for r in cls.rows],
                    "generated": {r["generated"]: r["symbol"] for r in cls.rows if "generated" in r}, "runtime_names": ["atexit"],
                    "discard_tables": identities.TABLE_DISCARD_RULE}
        cls.document = {"schema_version": 1, "units": {"func_80025C00": cls.unit},
                        "runtime_bindings": copy.deepcopy(identities.REVIEWED_RUNTIME)}
        reference = bytearray(EXPECTED_SIZE)
        reference[0x1000:0x107C] = TEXT
        reference[0xABF70:0xABF78] = bytes.fromhex("03e000080000102d")
        cls.reference = bytes(reference)
        attestation = patch("rom.attest", side_effect=lambda data: {"size": len(data), "sha256": hashlib.sha256(data).hexdigest()})
        attestation.start()
        cls.addClassCleanup(attestation.stop)
        cls.catalogue_rows = [{**{k: r[k] for k in ("image_id", "symbol", "rom_start", "vram_start", "size")},
                               "sha256": hashlib.sha256(cls.reference[r["rom_start"]:r["rom_start"] + r["size"]]).hexdigest()}
                              for r in cls.rows]
        cls.catalogue_rows.append({k: v for k, v in identities.REVIEWED_RUNTIME["atexit"].items() if k not in {"evidence", "evidence_sha256"}})
        cls.runs = [cls.prepare("first"), cls.prepare("replay")]
        cls.build_dir = cls.runs[0]
        for run in cls.runs:
            cls.compile(run)
            cert.generated_table_discards(run, cls.graph, cls.bound_profile(run), cls.reference)
            cls.command(run, cert.expected_commands(cls.graph, cls.bound_profile(run))[-2])
            (run / "shiren2.z64").write_bytes(cls.reference)
        cls.obj = match.Elf((cls.build_dir / OBJECT).read_bytes())
        cls.text = cls.obj.section(".text")
        cls.linked = match.Elf((cls.build_dir / "shiren2.elf").read_bytes())
        cls.code = cls.linked.contents(cls.linked.section(".text"))
        cls.reproduction = {"c_objects": {OBJECT: {"object_sha256": cert.sha256(cls.runs[1] / OBJECT)}},
                            "linked": {"shiren2.elf": cert.sha256(cls.runs[1] / "shiren2.elf")}}
        print(f"H22 native fixture evidence: {cls.work}", flush=True)

    @classmethod
    def prepare(cls, name: str) -> Path:
        run = cls.work / name
        for directory in ("source/src", "source/include", "source/config", "obj/source/src", "compiled/src", "generated", "tmp"):
            (run / directory).mkdir(parents=True, exist_ok=True)
        shutil.copyfile(ROOT / "tests/fixtures/generated_symbols/local_static.cpp", run / "source" / SOURCE)
        shutil.copyfile(ROOT / "config/generated-runtime-evidence.json", run / "source/config/generated-runtime-evidence.json")
        write_json(run / "source" / identities.CONFIG, cls.document)
        write_json(run / "source/config/rom_cpu_catalogue.json", {"identity": {"functions": cls.catalogue_rows}})
        write_json(run / "source/config/matches.json", {"functions": cls.rows})
        cls.write_images(run / "source", cls.reference)
        for path in ("generated/undefined_funcs_auto.txt", "generated/undefined_syms_auto.txt", "source/config/symbol_aliases.ld"):
            (run / path).write_text("")
        (run / "shiren2.ld").write_text(
            "SECTIONS {\n"
            f" .text 0x80025C00 : {{ {OBJECT}(.text) }}\n"
            f" .data 0x80039000 : {{ {OBJECT}(.data) }}\n"
            f" .bss 0x8003A000 (NOLOAD) : {{ {OBJECT}(.bss) }}\n"
            " /DISCARD/ : { *(*) }\n}\n"
            "release_object = 0x80030000;\nfunc_800D8F30 = 0x800D8F30;\nentrypoint = use_static;\n"
        )
        return run

    @classmethod
    def write_images(cls, root: Path, reference: bytes) -> None:
        inventory = json.loads((ROOT / "config/images.json").read_text())
        for image in inventory["images"]:
            image["rom_sha256"] = hashlib.sha256(reference[image["rom_start"]:image["rom_end"]]).hexdigest()
        write_json(root / "config/images.json", inventory)

    @classmethod
    def bound_profile(cls, run: Path) -> dict:
        return {**cls.profile, "tu_profiles": {SOURCE: PROFILE}, "symbol_identities": identities.profile_policy(run / "source")}

    @classmethod
    def command(cls, run: Path, argv: list[str]) -> None:
        logs = run / "commands.json"
        records = json.loads(logs.read_text()) if logs.exists() else []
        number = len(records)
        completed = subprocess.run(argv, cwd=run, env=cert.compiler_environment(run, cls.profile), capture_output=True, text=True)
        (run / f"command-{number}.stdout").write_text(completed.stdout)
        (run / f"command-{number}.stderr").write_text(completed.stderr)
        records.append({"argv": argv, "cwd": str(run), "exit_code": completed.returncode,
                        "stdout": f"command-{number}.stdout", "stderr": f"command-{number}.stderr"})
        write_json(logs, records)
        completed.check_returncode()

    @classmethod
    def compile(cls, run: Path) -> None:
        for command in cert.object_commands(OBJECT, cls.graph[OBJECT], cls.bound_profile(run))[1:]:
            cls.command(run, command)

    def changed_config(self, name: str, document: dict) -> Path:
        run = self.prepare(name)
        write_json(run / "source" / identities.CONFIG, document)
        return run / "source"

    def test_positive_native_local_static_exact_generated_bytes(self) -> None:
        self.assertEqual(self.code, TEXT, "pinned local-static code changed, not a regenerated golden")
        self.assertEqual((self.build_dir / OBJECT).read_bytes(), (self.runs[1] / OBJECT).read_bytes())
        self.assertEqual((self.build_dir / "shiren2.elf").read_bytes(), (self.runs[1] / "shiren2.elf").read_bytes())
        self.assertEqual(identities.object_problems(self.obj, self.text, self.rows, self.unit, cxx=True), [])
        self.assertEqual(match.cxx_object_problems(self.obj, self.text, self.unit["generated"]), [])
        records = identities.generated_evidence(self.obj, self.text, self.rows, self.unit, self.code, self.reference)
        self.assertEqual([(r["generated"], r["size"], r["matches_original"]) for r in records],
                         [("__tcf_0", 36, True), ("_GLOBAL_.I.use_static", 8, True)])
        write_json(self.build_dir / "generated-evidence.json", records)

    def test_unmapped_generated_symbol_is_rejected(self) -> None:
        unit = copy.deepcopy(self.unit)
        del unit["generated"]["__tcf_0"]
        problems = identities.object_problems(self.obj, self.text, self.rows, unit, cxx=True)
        self.assertTrue(any("unmapped extras forbidden" in problem for problem in problems))

    def test_non_allowlisted_pattern_is_rejected(self) -> None:
        document = copy.deepcopy(self.document)
        document["units"]["func_80025C00"]["generated"] = {"__udivdi3": "func_80025C00"}
        with self.assertRaisesRegex(ValueError, "Non-allowlisted"):
            identities.load(self.changed_config("bad-pattern", document))

    def test_absent_emitted_mapping_is_rejected(self) -> None:
        unit = copy.deepcopy(self.unit)
        unit["generated"]["__tcf_1"] = unit["generated"].pop("__tcf_0")
        self.assertTrue(identities.object_problems(self.obj, self.text, self.rows, unit, cxx=True))

    def test_generated_bytes_mismatch_is_rejected(self) -> None:
        code = bytearray(self.code)
        code[12] ^= 1
        records = identities.generated_evidence(self.obj, self.text, self.rows, self.unit, bytes(code), self.reference)
        self.assertFalse(records[0]["matches_original"])
        self.assertTrue(records[1]["matches_original"])

    def test_generated_wrong_rom_position_is_rejected(self) -> None:
        obj = copy.copy(self.obj)
        obj.symbols = [replace(s, value=s.value + 4) if s.name == "__tcf_0" else s for s in self.obj.symbols]
        self.assertTrue(identities.object_problems(obj, self.text, self.rows, self.unit, cxx=True))
        self.assertFalse(identities.generated_evidence(obj, self.text, self.rows, self.unit, self.code, self.reference)[0]["matches_original"])

    def test_hand_defined_catalogue_collision_is_rejected(self) -> None:
        run = self.prepare("hand-collision")
        source = run / "source" / SOURCE
        source.write_text(source.read_text() + '\nextern "C" void func_80025C00(void) {}\n')
        self.compile(run)
        obj = match.Elf((run / OBJECT).read_bytes())
        problems = identities.object_problems(obj, obj.section(".text"), self.rows, self.unit, cxx=True)
        self.assertTrue(any("Hand-defined catalogue name collision" in p for p in problems))

    def test_hand_written_compiler_name_is_rejected(self) -> None:
        source = self.work / "reserved.cpp"
        source.write_text('extern "C" void __tcf_0(void) {}\n')
        self.assertTrue(any("hand-written compiler-generated" in p for p in cert.source_file_violations(source, source.name)))

    def test_wrong_runtime_address_and_contract_are_rejected(self) -> None:
        for key, value in (("vram_start", 0x800D8F10), ("symbol", "func_800D8F10"), ("sha256", "0" * 64)):
            with self.subTest(field=key):
                document = copy.deepcopy(self.document)
                document["runtime_bindings"]["atexit"][key] = value
                with self.assertRaisesRegex(ValueError, "Unreviewed runtime binding or wrong address"):
                    identities.load(self.changed_config("wrong-runtime-" + key, document))
        with self.assertRaisesRegex(ValueError, "wrong link address"):
            identities.runtime_addresses(self.document, self.unit, {"func_800D8F30": 0x800D8F10})

    def test_undeclared_runtime_binding_is_rejected(self) -> None:
        document = copy.deepcopy(self.document)
        document["runtime_bindings"]["malloc"] = copy.deepcopy(identities.REVIEWED_RUNTIME["atexit"])
        with self.assertRaisesRegex(ValueError, "Unreviewed runtime binding"):
            identities.load(self.changed_config("undeclared-binding", document))
        document = copy.deepcopy(self.document)
        document["units"]["func_80025C00"]["runtime_names"] = ["malloc"]
        with self.assertRaisesRegex(ValueError, "Undeclared runtime name"):
            identities.load(self.changed_config("undeclared-unit-runtime", document))
        problems = identities.object_problems(self.obj, self.text, self.rows, {**self.unit, "runtime_names": []}, cxx=True)
        self.assertIn("Undeclared runtime name used by TU", problems)

    def test_runtime_evidence_and_rom_hash_are_bound(self) -> None:
        root = self.changed_config("changed-evidence", self.document)
        with (root / "config/generated-runtime-evidence.json").open("a") as stream:
            stream.write(" ")
        with self.assertRaisesRegex(ValueError, "review evidence changed"):
            identities.load(root)
        reference = bytearray(self.reference)
        reference[0xABF70] ^= 1
        with self.assertRaisesRegex(ValueError, "Runtime ROM evidence changed"):
            identities.load(self.build_dir / "source", bytes(reference))

    def test_pending_declaration_is_inert_for_partial_owner(self) -> None:
        old = [{**self.rows[-1], "source": "src/old.c"}]
        del old[0]["generated"]
        self.assertEqual(identities.active_units(self.document, old), {})
        self.assertEqual(identities.linker_flags({"symbol_identities": {"declaration": self.document, "active_units": {}}}, {}), [])
        with self.assertRaisesRegex(ValueError, "complete TU"):
            identities.unit_for(self.document, old)

    def test_manifest_cannot_hide_or_forge_generated_identity(self) -> None:
        rows = copy.deepcopy(self.rows)
        del rows[0]["generated"]
        with self.assertRaisesRegex(ValueError, "Manifest generated identity"):
            identities.active_units(self.document, rows)
        rows = copy.deepcopy(self.rows)
        rows[0]["source"] = "src/invalid.c"
        with self.assertRaisesRegex(ValueError, r"invalid generated C\+\+ identity"):
            validate_matches(rows, IMAGE)

    def test_receipt_proves_emitter_and_counts_subset_once(self) -> None:
        with patch("certification.attest"):
            records = cert.c_attributions(self.build_dir, self.rows, self.graph, self.bound_profile(self.build_dir), self.reference,
                                         reproduction=self.reproduction)
        validate_attributions(self.rows, records, self.graph)
        self.assertEqual([(r["symbol"], r.get("generated")) for r in records], [(r["symbol"], r.get("generated")) for r in self.rows])
        self.assertTrue(all(r["runtime_bindings"]["atexit"] == identities.REVIEWED_RUNTIME["atexit"] for r in records))
        self.assertTrue(all(r["discarded_tables"][0]["section"] == ".ctors" for r in records))
        self.assertTrue(all(r["discarded_tables"][0]["source_credit_bytes"] == 0 for r in records))
        counts = validate_matches(self.rows, IMAGE)
        self.assertEqual((counts["matched_cpp_functions"], counts["matched_cpp_bytes"]), (3, 124))
        self.assertEqual((counts["matched_generated_cpp_functions"], counts["matched_generated_cpp_bytes"]), (2, 44))
        self.assertEqual(counts["matched_c_and_cpp_bytes"], 124)
        write_json(self.build_dir / "fixture-receipt.json", {"scope": "synthetic miniature link, NOT full-ROM acceptance", "model": "gpt-6-astra", "family": "OpenAI",
                                                           "coverage": counts, "c_matches": self.rows, "c_attributions": records,
                                                           "profile": self.bound_profile(self.build_dir), "reproduction": self.reproduction})
        forged = copy.deepcopy(records)
        forged[0]["generated"] = "__tcf_9"
        with self.assertRaisesRegex(ValueError, "generated identity disagrees"):
            validate_attributions(self.rows, forged, self.graph)

    def test_receipt_rejects_link_alias_with_wrong_address(self) -> None:
        run = self.prepare("wrong-linked-alias")
        self.compile(run)
        command = cert.expected_commands(self.graph, self.bound_profile(run))[-2]
        command = ["--defsym=atexit=0x800D8F10" if arg.startswith("--defsym=atexit=") else arg for arg in command]
        self.command(run, command)
        (run / "shiren2.z64").write_bytes(self.reference)
        with patch("certification.attest"), self.assertRaisesRegex(ValueError, "wrong link address"):
            cert.c_attributions(run, self.rows, self.graph, self.bound_profile(run), self.reference, reproduction=self.reproduction)

    def test_native_ctor_table_discard_is_proven_without_placement(self) -> None:
        records = cert.generated_table_discards(self.build_dir, self.graph, self.bound_profile(self.build_dir), self.reference)[OBJECT]
        self.assertEqual([(r["section"], r["size"], r["disposition"]) for r in records], [(".ctors", 4, "discard")])
        self.assertEqual(records[0]["entries"][0]["symbol"], "func_80025C74")
        self.assertEqual({s["image_id"] for s in records[0]["scanned_images"]},
                         {"resident", "main_14400", "overlay_1339f0", "overlay_136dc0"})
        self.assertEqual(records[0]["scanned_rom"], {"rom_start": 0, "rom_end": EXPECTED_SIZE,
                                                   "sha256": hashlib.sha256(self.reference).hexdigest()})
        self.assertIsNone(self.linked.section(".ctors"))
        identities.prove_linker_discard((self.build_dir / "shiren2.map").read_text(), OBJECT, records)
        write_json(self.build_dir / "table-discard-evidence.json", records)

    def test_native_global_static_finalizer_table_is_supported(self) -> None:
        run = self.prepare("global-static")
        shutil.copyfile(ROOT / "tests/fixtures/generated_symbols/global_static.cpp", run / "source" / SOURCE)
        self.compile(run)
        obj = match.Elf((run / OBJECT).read_bytes())
        text = obj.section(".text")
        functions = sorted(identities.emitted_functions(obj, text).values(), key=lambda s: s.value)
        rows, mapping = [], {}
        reference = bytearray(self.reference)
        reference[0x1000:0x1000 + text.size] = obj.contents(text)
        for index, symbol in enumerate(functions):
            target = f"fixture_generated_{index}" if identities.GENERATED.fullmatch(symbol.name) else symbol.name
            row = {"image_id": "resident", "symbol": target, "rom_start": 0x1000 + symbol.value,
                   "vram_start": 0x80025C00 + symbol.value, "size": symbol.size,
                   "sha256": hashlib.sha256(obj.contents(text)[symbol.value:symbol.value + symbol.size]).hexdigest()}
            rows.append(row)
            if target != symbol.name:
                mapping[symbol.name] = target
        unit = {"image_id": "resident", "functions": [r["symbol"] for r in rows], "generated": mapping,
                "runtime_names": [], "discard_tables": identities.TABLE_DISCARD_RULE}
        document = {"schema_version": 1, "units": {rows[0]["symbol"]: unit}, "runtime_bindings": {}}
        write_json(run / "source" / identities.CONFIG, document)
        write_json(run / "source/config/rom_cpu_catalogue.json", {"identity": {"functions": rows}})
        self.write_images(run / "source", bytes(reference))
        identities.load(run / "source", bytes(reference))
        records = identities.table_discards(run / "source", obj, unit, bytes(reference))
        self.assertIn(".dtors", [r["section"] for r in records])
        self.assertTrue(all(entry["generated"].startswith(identities.TABLE_PREFIXES[r["section"]])
                            for r in records for entry in r["entries"]))
        write_json(run / "table-discard-evidence.json", records)

    def assert_table_word_rejected(self, offset: int) -> None:
        run = self.prepare(f"existing-table-{offset:x}")
        reference = bytearray(self.reference)
        reference[offset:offset + 4] = self.rows[-1]["vram_start"].to_bytes(4, "big")
        self.write_images(run / "source", bytes(reference))
        with self.assertRaisesRegex(ValueError, "target word exists in ROM"):
            identities.table_discards(run / "source", self.obj, self.unit, bytes(reference))

    def test_table_word_in_header_or_ipl3_refuses_discard(self) -> None:
        for offset in (0, 0x40, 0x41, 0xFFC):
            with self.subTest(rom=hex(offset)):
                self.assert_table_word_rejected(offset)

    def test_table_word_in_stored_tail_refuses_discard(self) -> None:
        for offset in (0x148460, 0x148461, 0x1FFFFC, EXPECTED_SIZE - 4):
            with self.subTest(rom=hex(offset)):
                self.assert_table_word_rejected(offset)

    def test_table_discard_requires_real_whole_rom_attestation(self) -> None:
        # A complete, table-free synthetic ROM is still not the fixed game ROM.
        with patch("rom.attest", real_rom_attest), self.assertRaisesRegex(ValueError, "Wrong target"):
            identities.table_discards(self.build_dir / "source", self.obj, self.unit, self.reference)

    def test_table_word_in_any_image_including_main_init_tables_refuses_discard(self) -> None:
        # Both known main-image init-table regions must be scanned even for a
        # foreign TU; the native fixture owner is resident, not main_14400.
        for offset in (0x1200, 0x11BEC0, 0x11D03C, 0x136DC0):
            with self.subTest(rom=hex(offset)):
                self.assert_table_word_rejected(offset)

    def test_table_discard_requires_declared_rule_and_original_complete_images(self) -> None:
        for unit in ({}, {k: v for k, v in self.unit.items() if k != "discard_tables"}):
            with self.assertRaisesRegex(ValueError, "Undeclared"):
                identities.table_discards(self.build_dir / "source", self.obj, unit, self.reference)
        with self.assertRaisesRegex(ValueError, "complete original 32 MiB ROM"):
            identities.table_discards(self.build_dir / "source", self.obj, self.unit, self.reference[:-4])
        changed = bytearray(self.reference)
        changed[0x14400] ^= 1
        with self.assertRaisesRegex(ValueError, "complete original image"):
            identities.table_discards(self.build_dir / "source", self.obj, self.unit, bytes(changed))
        document = copy.deepcopy(self.document)
        document["units"]["func_80025C00"]["discard_tables"] = "unreviewed"
        with self.assertRaisesRegex(ValueError, "Unreviewed generated table"):
            identities.load(self.changed_config("bad-discard-rule", document))

    def test_table_discard_rejects_wrong_target_relocation_and_incoming_reference(self) -> None:
        table = self.obj.section(".ctors")
        raw = bytearray(self.obj.data)
        raw[table.offset:table.offset + 4] = (self.rows[1]["rom_start"] - 0x1000).to_bytes(4, "big")
        with self.assertRaisesRegex(ValueError, "mapped generated initializer"):
            identities.table_discards(self.build_dir / "source", match.Elf(bytes(raw)), self.unit, self.reference)
        for failure in ("relocation", "incoming"):
            obj = copy.copy(self.obj)
            def relocations(section: match.Section) -> list:
                records = self.obj.relocations(section)
                if failure == "relocation" and section.index == table.index:
                    return [(offset, match.R_MIPS_26, symbol) for offset, _, symbol in records]
                if failure == "incoming" and section.name == ".data":
                    return [(0, match.R_MIPS_32, match.Symbol("table", 0, 0, 0, 3, table.index))]
                return records
            obj.relocations = relocations
            with self.subTest(failure=failure), self.assertRaisesRegex(ValueError, "R_MIPS_32|incoming references"):
                identities.table_discards(self.build_dir / "source", obj, self.unit, self.reference)

    def test_table_discard_rejects_named_storage_and_section_suffixes(self) -> None:
        table = self.obj.section(".ctors")
        obj = copy.copy(self.obj)
        obj.symbols = [*self.obj.symbols, match.Symbol("exported", 0, 4, 1, 1, table.index)]
        with self.assertRaisesRegex(ValueError, "named or exported"):
            identities.table_discards(self.build_dir / "source", obj, self.unit, self.reference)
        obj = copy.copy(self.obj)
        obj.sections = [replace(s, name=".ctors.65535") if s.index == table.index else s for s in self.obj.sections]
        with self.assertRaisesRegex(ValueError, "Unsupported compiler table"):
            identities.table_discards(self.build_dir / "source", obj, self.unit, self.reference)

    def test_receipt_refuses_a_table_placed_instead_of_discarded(self) -> None:
        run = self.prepare("placed-table")
        script = run / "shiren2.ld"
        script.write_text(script.read_text().replace(" /DISCARD/", f" .ctors 0x80039100 : {{ {OBJECT}(.ctors) }}\n /DISCARD/"))
        self.compile(run)
        self.command(run, cert.expected_commands(self.graph, self.bound_profile(run))[-2])
        (run / "shiren2.z64").write_bytes(self.reference)
        with patch("certification.attest"), self.assertRaisesRegex(ValueError, "not exclusively discarded"):
            cert.c_attributions(run, self.rows, self.graph, self.bound_profile(run), self.reference)

    def test_progress_lists_generated_functions_and_emitting_tu(self) -> None:
        counts = validate_matches(self.rows, IMAGE)
        identity = {"functions": self.catalogue_rows[:3], "images": [{"image_id": "resident", **IMAGE["resident"]}]}
        audit = {"functions": 3, "instruction_bytes": 124, "images": {"resident": {"functions": 3, "instruction_bytes": 124,
                                                                                 "gap_bytes": 0, "zero_words_inside_functions": 0, "handwritten_annotated_bytes": 0}}}
        denominator = {"schema_version": 1, "identity": identity, "denominator_id": cert.json_digest(identity),
                       "audit": audit, "origin": {"accepted_receipt": {}, "validation": {}}}
        accepted = SimpleNamespace(root=self.build_dir, original=self.reference,
                                   inventory=SimpleNamespace(images={"resident": SimpleNamespace(**IMAGE["resident"])}),
                                   document={"coverage": counts, "c_matches": self.rows,
                                             "c_attributions": [{"image_id": r["image_id"], "symbol": r["symbol"], "reference_sha256": c["sha256"]}
                                                                for r, c in zip(self.rows, self.catalogue_rows)]})
        with patch("progress._read_reference"), patch("progress.audit_catalogue", return_value=audit):
            coverage = progress.calculate(accepted, denominator)
        self.assertEqual(coverage["matched_generated_cpp_instruction_bytes"], 44)
        self.assertEqual([r["generated"] for r in coverage["generated_functions"]], ["__tcf_0", "_GLOBAL_.I.use_static"])
        accounting = {"mapped_catalogue_accounted_percent": 100, "classified_handwritten_functions": 0,
                      "classified_handwritten_instruction_bytes": 0, "accounted_instruction_bytes": 124,
                      "validated_denominator_id": "fixture", "images": {"resident": {"matched_c_instruction_bytes": 0,
                      "matched_cpp_instruction_bytes": 124, "classified_handwritten_instruction_bytes": 0,
                      "accounted_instruction_bytes": 124, "provisional_catalogued_instruction_bytes": 124, "mapped_catalogue_accounted_percent": 100}}}
        report = {"coverage": coverage, "accounting": accounting, "freshness": {"current_tree_matches_accepted_snapshot": True},
                  "accepted_receipt": {"path": "fixture-receipt.json", "sha256": "test-only"}, "validation": {"path": "fixture", "sha256": "test-only"},
                  "denominator": {"id": "fixture", "path": "fixture"}, "handwritten_classification": {"path": "fixture", "sha256": "test-only"}, "catalogue_audit": audit}
        rendered = progress.markdown(report)
        self.assertIn("generated | `func_80025C00` | `__tcf_0` | `src/local_static.cpp` | 36", rendered)
        self.assertIn("included above, not extra credit", rendered)
        write_json(self.build_dir / "fixture-progress.json", report)
        (self.build_dir / "fixture-progress.md").write_text(rendered)


if __name__ == "__main__":
    unittest.main(verbosity=2)
