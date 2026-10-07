from __future__ import annotations

import copy
import hashlib
import json
import os
import sys
import tempfile
import unittest
from pathlib import Path
from typing import Any
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import workspace
from workspace import create_snapshot, input_manifest, verify_manifest


class WorkspaceTests(unittest.TestCase):
    def setUp(self) -> None:
        self.temporary = tempfile.TemporaryDirectory()
        self.addCleanup(self.temporary.cleanup)
        self.base = Path(self.temporary.name).resolve()
        self.root = self.base / "project"
        self.root.mkdir()
        for name in workspace.REQUIRED_FILES:
            (self.root / name).write_text(f"fixture {name}\n", encoding="utf-8")
        for name in workspace.SOURCE_DIRECTORIES:
            (self.root / name).mkdir()
        self.write("src/nested/function.c", "int fixture(void) { return 3; }\n")
        self.write("include/nested/common.h", "#define VALUE 3\n")
        self.write("config/nested/layout.json", "{}\n")
        self.write("tools/nested/build.py", "print('fixture')\n")
        self.write("tests/nested/check.py", "assert 3 == 3\n")

    def write(self, relative: str, text: str) -> Path:
        path = self.root / relative
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(text, encoding="utf-8")
        return path

    def replace_digest(self, manifest: dict[str, Any]) -> None:
        encoded = json.dumps(manifest["files"], sort_keys=True, separators=(",", ":"), ensure_ascii=True)
        manifest["sha256"] = hashlib.sha256(encoded.encode("utf-8")).hexdigest()

    def test_complete_deterministic_allowlist_includes_untracked_nested_files(self) -> None:
        self.write("README.md", "fixture readme\n")
        self.write("AGENTS.md", "fixture instructions\n")
        manifest = input_manifest(self.root)
        self.assertEqual(manifest, input_manifest(self.root))
        self.assertEqual(manifest["schema_version"], 1)
        self.assertEqual(list(manifest["files"]), sorted(manifest["files"]))
        expected = set(workspace.REQUIRED_FILES) | {
            "README.md", "AGENTS.md", "src/nested/function.c", "include/nested/common.h",
            "config/nested/layout.json", "tools/nested/build.py", "tests/nested/check.py",
        }
        self.assertEqual(set(manifest["files"]), expected)
        source = (self.root / "src/nested/function.c").read_bytes()
        self.assertEqual(manifest["files"]["src/nested/function.c"], {
            "sha256": hashlib.sha256(source).hexdigest(), "size": len(source),
        })
        self.replace_digest(manifest)
        verify_manifest(self.root, manifest)

    def test_outputs_metadata_docs_and_other_experiments_are_excluded(self) -> None:
        for relative in [
            ".DS_Store", ".git/config", ".venv/secret", "scratch/candidate.c",
            "build/receipt.json", "generated/source.c", ".cache/tool", "docs/status.md",
            "inputs/shiren2/baserom.z64",
            "another-project/input.c", "src/.DS_Store", "include/__pycache__/cache.pyc",
            "tools/nested/.git/config", "src/nested/build/output.c",
        ]:
            self.write(relative, "excluded\n")
        before = input_manifest(self.root)
        self.write("docs/status.md", "updated while copying\n")
        self.write("build/receipt.json", "updated output\n")
        self.assertEqual(before, input_manifest(self.root))
        self.assertEqual(len(before["files"]), len(workspace.REQUIRED_FILES) + 5)

    def test_snapshot_is_exclusive_and_copies_verified_bytes(self) -> None:
        source = self.root / "tools/nested/build.py"
        source.chmod(0o755)
        destination = self.root / "scratch/run/source"
        manifest = create_snapshot(self.root, destination)
        verify_manifest(self.root, manifest)
        verify_manifest(destination, manifest)
        for relative in manifest["files"]:
            self.assertEqual((self.root / relative).read_bytes(), (destination / relative).read_bytes())
        self.assertEqual((destination / "tools/nested/build.py").stat().st_mode & 0o777, 0o755)
        self.assertFalse((destination / "scratch").exists())
        with self.assertRaises(FileExistsError):
            create_snapshot(self.root, destination)

    def test_existing_empty_destination_is_not_reused(self) -> None:
        destination = self.base / "existing"
        destination.mkdir()
        with self.assertRaises(FileExistsError):
            create_snapshot(self.root, destination)
        self.assertEqual(list(destination.iterdir()), [])

    def test_snapshot_bytes_survive_later_live_source_edit(self) -> None:
        destination = self.base / "snapshot"
        manifest = create_snapshot(self.root, destination)
        original = (destination / "src/nested/function.c").read_bytes()
        self.write("src/nested/function.c", "int fixture(void) { return 4; }\n")
        self.assertEqual((destination / "src/nested/function.c").read_bytes(), original)
        verify_manifest(destination, manifest)
        with self.assertRaisesRegex(ValueError, "changed=.*function.c"):
            verify_manifest(self.root, manifest)

    def test_source_addition_removal_and_mutation_fail_complete_verification(self) -> None:
        for mutation in ("add", "remove", "change"):
            with self.subTest(mutation=mutation):
                manifest = input_manifest(self.root)
                source = self.root / "include/nested/common.h"
                original = source.read_bytes()
                if mutation == "add":
                    extra = self.write("include/nested/extra.h", "extra\n")
                elif mutation == "remove":
                    source.unlink()
                else:
                    source.write_bytes(original + b"changed\n")
                with self.assertRaisesRegex(ValueError, "Input manifest mismatch"):
                    verify_manifest(self.root, manifest)
                if mutation == "add":
                    extra.unlink()
                else:
                    source.write_bytes(original)

    def test_missing_mandatory_file_fails_manifest_and_snapshot(self) -> None:
        (self.root / "uv.lock").unlink()
        with self.assertRaisesRegex(ValueError, "uv.lock"):
            input_manifest(self.root)
        destination = self.base / "snapshot"
        with self.assertRaises(ValueError):
            create_snapshot(self.root, destination)
        self.assertFalse(destination.exists())

    def test_mandatory_file_cannot_be_replaced_by_directory_or_symlink(self) -> None:
        required = self.root / "uv.lock"
        required.unlink()
        required.mkdir()
        with self.assertRaises(ValueError):
            input_manifest(self.root)
        required.rmdir()
        required.symlink_to(self.root / "pyproject.toml")
        with self.assertRaises(ValueError):
            input_manifest(self.root)

    def test_manifest_schema_digest_and_fingerprint_are_validated(self) -> None:
        valid = input_manifest(self.root)
        mutations = []
        for field, value in [("schema_version", 2), ("schema_version", True), ("sha256", "0" * 64)]:
            changed = copy.deepcopy(valid)
            changed[field] = value
            mutations.append(changed)
        changed = copy.deepcopy(valid)
        changed["extra"] = "unsupported"
        mutations.append(changed)
        changed = copy.deepcopy(valid)
        del changed["files"]["uv.lock"]
        self.replace_digest(changed)
        mutations.append(changed)
        for fingerprint in [{"sha256": "invalid", "size": 1}, {"sha256": "0" * 64, "size": True}, {"sha256": "0" * 64, "size": -1}, {"sha256": "0" * 64}]:
            changed = copy.deepcopy(valid)
            changed["files"]["Makefile"] = fingerprint
            self.replace_digest(changed)
            mutations.append(changed)
        for manifest in mutations:
            with self.subTest(manifest=manifest):
                with self.assertRaises(ValueError):
                    verify_manifest(self.root, manifest)

    def test_self_consistent_omitted_and_extra_manifest_entries_fail(self) -> None:
        valid = input_manifest(self.root)
        omitted = copy.deepcopy(valid)
        del omitted["files"]["src/nested/function.c"]
        self.replace_digest(omitted)
        with self.assertRaisesRegex(ValueError, "extra=.*function.c"):
            verify_manifest(self.root, omitted)
        extra = copy.deepcopy(valid)
        extra["files"]["src/not-present.c"] = {"sha256": "0" * 64, "size": 0}
        self.replace_digest(extra)
        with self.assertRaisesRegex(ValueError, "missing=.*not-present.c"):
            verify_manifest(self.root, extra)

    def test_noncanonical_escaping_or_undeclared_manifest_paths_fail(self) -> None:
        for relative in ("../outside", "/outside", "src/../../outside", "src//file.c", "./Makefile", "src\\file.c", "", "docs/status.md", "src/build/file.c"):
            with self.subTest(relative=relative):
                manifest = input_manifest(self.root)
                manifest["files"][relative] = {"sha256": "0" * 64, "size": 0}
                self.replace_digest(manifest)
                with self.assertRaises(ValueError):
                    verify_manifest(self.root, manifest)

    def test_symlinked_selected_file_directory_and_root_are_rejected(self) -> None:
        for target in (self.root / "src/nested/function.c", self.base / "outside.c", self.base / "absent.c"):
            with self.subTest(target=target):
                link = self.root / "include/link.h"
                link.symlink_to(target)
                with self.assertRaises(ValueError):
                    input_manifest(self.root)
                link.unlink()
        link = self.root / "src/link"
        link.symlink_to(self.root / "include", target_is_directory=True)
        with self.assertRaises(ValueError):
            input_manifest(self.root)
        link.unlink()
        link = self.base / "linked-root"
        link.symlink_to(self.root, target_is_directory=True)
        with self.assertRaises(ValueError):
            input_manifest(link)

    def test_nonregular_selected_file_is_rejected_without_reading(self) -> None:
        fifo = self.root / "src/fifo"
        os.mkfifo(fifo)
        with self.assertRaisesRegex(ValueError, "regular file"):
            input_manifest(self.root)

    def test_snapshot_cannot_live_inside_allowlisted_sources(self) -> None:
        for relative in ("src/private", "include/private", "tools/private"):
            with self.subTest(relative=relative):
                destination = self.root / relative
                with self.assertRaisesRegex(ValueError, "outside the source allowlist"):
                    create_snapshot(self.root, destination)
                self.assertFalse(destination.exists())

    def test_destination_symlink_parent_and_traversal_are_rejected(self) -> None:
        link = self.root / "scratch-link"
        link.symlink_to(self.base, target_is_directory=True)
        with self.assertRaises(ValueError):
            create_snapshot(self.root, link / "snapshot")
        with self.assertRaises(ValueError):
            create_snapshot(self.root, link)
        with self.assertRaises(ValueError):
            create_snapshot(self.root, self.root / "scratch/../snapshot")
        self.assertFalse((self.base / "snapshot").exists())

    def test_input_change_before_copy_rejects_partial_snapshot(self) -> None:
        original_copy = workspace._copy_file
        changed = False

        def racing_copy(source: Path, destination: Path, expected: workspace.FileFingerprint) -> None:
            nonlocal changed
            if not changed and source.name == "function.c":
                changed = True
                source.write_text("changed before copy\n", encoding="utf-8")
            original_copy(source, destination, expected)

        destination = self.base / "snapshot"
        with patch.object(workspace, "_copy_file", side_effect=racing_copy):
            with self.assertRaisesRegex(ValueError, "changed before copying"):
                create_snapshot(self.root, destination)
        self.assertTrue(destination.is_dir())

    def test_already_copied_source_change_fails_live_postcopy_check(self) -> None:
        original_copy = workspace._copy_file
        changed = False

        def racing_copy(source: Path, destination: Path, expected: workspace.FileFingerprint) -> None:
            nonlocal changed
            original_copy(source, destination, expected)
            if not changed:
                changed = True
                source.write_bytes(source.read_bytes() + b"late edit\n")

        destination = self.base / "snapshot"
        with patch.object(workspace, "_copy_file", side_effect=racing_copy):
            with self.assertRaisesRegex(ValueError, "Input manifest mismatch"):
                create_snapshot(self.root, destination)
        self.assertTrue(destination.is_dir())

    def test_extra_live_or_frozen_input_during_copy_is_rejected(self) -> None:
        original_copy = workspace._copy_file
        for target in ("live", "frozen"):
            with self.subTest(target=target):
                added = False
                destination_root = self.base / f"snapshot-{target}"

                def racing_copy(source: Path, destination: Path, expected: workspace.FileFingerprint) -> None:
                    nonlocal added
                    original_copy(source, destination, expected)
                    if not added:
                        added = True
                        root = self.root if target == "live" else destination_root
                        extra = root / "src/extra.c"
                        extra.parent.mkdir(parents=True, exist_ok=True)
                        extra.write_text("extra input\n", encoding="utf-8")

                with patch.object(workspace, "_copy_file", side_effect=racing_copy):
                    with self.assertRaisesRegex(ValueError, "Input manifest mismatch"):
                        create_snapshot(self.root, destination_root)
                if target == "live":
                    (self.root / "src/extra.c").unlink()

    def test_corrupted_frozen_file_is_rejected_even_when_live_source_is_unchanged(self) -> None:
        original_copy = workspace._copy_file
        corrupted = False

        def corrupting_copy(source: Path, destination: Path, expected: workspace.FileFingerprint) -> None:
            nonlocal corrupted
            original_copy(source, destination, expected)
            if not corrupted:
                corrupted = True
                destination.write_bytes(destination.read_bytes() + b"corrupted snapshot\n")

        destination = self.base / "snapshot"
        with patch.object(workspace, "_copy_file", side_effect=corrupting_copy):
            with self.assertRaisesRegex(ValueError, "Input manifest mismatch"):
                create_snapshot(self.root, destination)


if __name__ == "__main__":
    unittest.main()
