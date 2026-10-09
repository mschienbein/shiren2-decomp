from __future__ import annotations

import copy
import hashlib
import importlib.util
import io
import json
import subprocess
import tarfile
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch


PROJECT = Path(__file__).resolve().parents[1]
# Avoid importing an unrelated site-package named setup.
MODULE_SPEC = importlib.util.spec_from_file_location("harness_setup", PROJECT / "tools/setup.py")
assert MODULE_SPEC is not None and MODULE_SPEC.loader is not None
setup = importlib.util.module_from_spec(MODULE_SPEC)
MODULE_SPEC.loader.exec_module(setup)


def digest(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


class GasProvisioningTests(unittest.TestCase):
    ORIGINAL = b"first\nold backend\nlast\n"
    MODIFIED = b"first\nnew backend\nlast\n"
    PATCH = b"--- a/gas/config/tc-mips.c\n+++ b/gas/config/tc-mips.c\n@@ -1,3 +1,3 @@\n first\n-old backend\n+new backend\n last\n"

    def setUp(self) -> None:
        self.temporary = tempfile.TemporaryDirectory()
        self.addCleanup(self.temporary.cleanup)
        self.root = Path(self.temporary.name)
        self.cache = self.root / ".cache"
        self.cache.mkdir()
        self.source = self.root / "source"
        self.backend = self.source / "gas/config/tc-mips.c"
        self.backend.parent.mkdir(parents=True)
        self.backend.write_bytes(self.ORIGINAL)
        self.patch_path = self.root / "tools/patches/fp32.patch"
        self.patch_path.parent.mkdir(parents=True)
        self.patch_path.write_bytes(self.PATCH)
        self.spec = {
            "version": "2.9.1",
            "install_directory": ".cache/binutils-gnu-2.9.1-fp32",
            "assembler_sha256": digest(b"pinned gas"),
            "source_patch": {
                "path": "tools/patches/fp32.patch",
                "sha256": digest(self.PATCH),
                "source_path": "gas/config/tc-mips.c",
                "original_sha256": digest(self.ORIGINAL),
                "modified_sha256": digest(self.MODIFIED),
            },
        }
        for name, value in (("PROJECT", self.root), ("CACHE", self.cache)):
            replacement = patch.object(setup, name, value)
            replacement.start()
            self.addCleanup(replacement.stop)

    def test_cached_archive_hash_mismatch_is_refused_without_download(self) -> None:
        archive = self.cache / "binutils-2.9.1.tar.gz"
        archive.write_bytes(b"tampered archive")
        with patch.object(setup, "urlopen") as request:
            with self.assertRaisesRegex(ValueError, "Archive hash mismatch"):
                setup.download({"url": "https://example.invalid/archive", "sha256": digest(b"expected")}, archive.name)
            request.assert_not_called()

    def test_downloaded_archive_hash_mismatch_is_refused(self) -> None:
        with patch.object(setup, "urlopen", return_value=io.BytesIO(b"wrong archive")) as request:
            with self.assertRaisesRegex(ValueError, "Archive hash mismatch"):
                setup.download({"url": "https://example.invalid/archive", "sha256": digest(b"expected")}, "archive.tar.gz")
            request.assert_called_once()

    def test_patch_digest_is_checked_before_backend_mutation(self) -> None:
        self.patch_path.write_bytes(self.PATCH + b"tampered\n")
        with self.assertRaises(ValueError):
            setup.gas_source_patch(self.spec)
        with self.assertRaises(ValueError):
            setup.apply_gas_source_patch(self.source, self.spec)
        self.assertEqual(self.backend.read_bytes(), self.ORIGINAL)

    def test_original_backend_digest_mismatch_is_refused(self) -> None:
        self.backend.write_bytes(b"unexpected backend\n")
        with self.assertRaises(ValueError):
            setup.apply_gas_source_patch(self.source, self.spec)
        self.assertEqual(self.backend.read_bytes(), b"unexpected backend\n")

    def test_modified_backend_digest_mismatch_is_refused(self) -> None:
        self.spec["source_patch"]["modified_sha256"] = "0" * 64
        with self.assertRaises(ValueError):
            setup.apply_gas_source_patch(self.source, self.spec)

    def test_verified_patch_changes_exact_backend_bytes(self) -> None:
        self.assertEqual(setup.gas_source_patch(self.spec), self.PATCH)
        setup.apply_gas_source_patch(self.source, self.spec)
        self.assertEqual(self.backend.read_bytes(), self.MODIFIED)
        # Reapplication cannot silently accept an already-modified source tree.
        with self.assertRaises(ValueError):
            setup.apply_gas_source_patch(self.source, self.spec)

    def test_patch_cannot_ignore_mismatched_context_with_fuzz(self) -> None:
        original = b"different first\nold backend\nlast\n"
        modified = b"different first\nnew backend\nlast\n"
        self.backend.write_bytes(original)
        self.spec["source_patch"].update(
            original_sha256=digest(original), modified_sha256=digest(modified),
        )
        # Both digests are valid for this fixture; only zero-fuzz application
        # prevents accepting a patch whose first context line does not match.
        with self.assertRaises((ValueError, subprocess.CalledProcessError)):
            setup.apply_gas_source_patch(self.source, self.spec)
        self.assertEqual(self.backend.read_bytes(), original)

    def test_stock_recipe_does_not_read_or_change_backend(self) -> None:
        stock = {key: value for key, value in self.spec.items() if key != "source_patch"}
        stock["install_directory"] = ".cache/binutils-gnu-2.9.1"
        self.patch_path.unlink()
        self.assertIsNone(setup.gas_source_patch(stock))
        setup.apply_gas_source_patch(self.source, stock)
        self.assertEqual(self.backend.read_bytes(), self.ORIGINAL)
        installed = self.root / stock["install_directory"] / "as"
        installed.parent.mkdir(parents=True)
        installed.write_bytes(b"pinned gas")
        with patch.object(setup, "download") as download, patch.object(setup.subprocess, "run") as run:
            self.assertEqual(setup.install_gnu_gas(stock, "unused-make"), installed)
            download.assert_not_called()
            run.assert_not_called()

    def test_installed_patched_assembler_still_requires_patch_digest(self) -> None:
        installed = self.root / self.spec["install_directory"] / "as"
        installed.parent.mkdir(parents=True)
        installed.write_bytes(b"pinned gas")
        self.patch_path.write_bytes(self.PATCH + b"tampered\n")
        with patch.object(setup, "download") as download, patch.object(setup.subprocess, "run") as run:
            with self.assertRaises(ValueError):
                setup.install_gnu_gas(self.spec, "unused-make")
            download.assert_not_called()
            run.assert_not_called()

    def test_installed_patched_assembler_digest_mismatch_is_refused(self) -> None:
        installed = self.root / self.spec["install_directory"] / "as"
        installed.parent.mkdir(parents=True)
        installed.write_bytes(b"tampered gas")
        with self.assertRaisesRegex(ValueError, "differs from the lock"):
            setup.install_gnu_gas(self.spec, "unused-make")

    def test_main_uses_the_locked_fp32_make_executable(self) -> None:
        assembler = self.cache / "binutils/bin/mips64-elf-as"
        assembler.parent.mkdir(parents=True)
        assembler.write_bytes(b"installed binutils")
        kmc = self.cache / "binutils-kmc-2.6/as"
        kmc.parent.mkdir()
        kmc.write_bytes(b"installed kmc")
        locked_make = "/private/locked-toolchain/make"
        lock = {
            "gcc_candidate": {}, "gcc_pm281": {}, "clang_ast": {},
            "binutils_kmc": {"install_directory": ".cache/binutils-kmc-2.6",
                             "assembler_sha256": digest(kmc.read_bytes())},
            "binutils_gnu291": {"version": "stock"},
            "binutils_gnu291_fp32": {"version": "fp32", "make_executable": locked_make},
        }
        (self.root / "toolchain.lock.json").write_text(json.dumps(lock))
        with patch.object(setup.platform, "system", return_value="Darwin"), \
                patch.object(setup.shutil, "which", return_value="/private/discovered/gmake"), \
                patch.object(setup, "install_prebuilt_compiler", return_value=self.root), \
                patch.object(setup, "install_gnu_gas", return_value=assembler) as install, \
                patch.object(setup, "install_clang_ast", return_value=assembler), \
                patch.object(setup.subprocess, "check_output", return_value="tool version\n"), \
                patch("sys.stdout", new_callable=io.StringIO):
            setup.main()
        self.assertEqual(install.call_args_list, [
            unittest.mock.call(lock["binutils_gnu291"], "/private/discovered/gmake"),
            unittest.mock.call(lock["binutils_gnu291_fp32"], locked_make),
        ])

    def test_reviewed_patch_applies_to_pinned_archive_backend(self) -> None:
        lock = json.loads((PROJECT / "toolchain.lock.json").read_text())
        spec = copy.deepcopy(lock["binutils_gnu291_fp32"])
        archive = PROJECT / ".cache" / f"binutils-{spec['version']}.tar.gz"
        if not archive.is_file():
            self.skipTest("Pinned GNU 2.9.1 source archive is absent")
        self.assertEqual(digest(archive.read_bytes()), spec["sha256"])
        self.assertEqual(spec["sha256"], lock["binutils_gnu291"]["sha256"])
        self.assertEqual(spec["host_patches"], lock["binutils_gnu291"]["host_patches"])
        metadata = spec["source_patch"]
        target = self.source / metadata["source_path"]
        target.parent.mkdir(parents=True, exist_ok=True)
        with tarfile.open(archive) as tar:
            member = tar.extractfile(f"binutils-{spec['version']}/{metadata['source_path']}")
            self.assertIsNotNone(member)
            assert member is not None
            with member:
                target.write_bytes(member.read())
        with patch.object(setup, "PROJECT", PROJECT):
            self.assertEqual(digest(setup.gas_source_patch(spec)), "c9c8bb8b0db81320a3579d5f726f75b34e805d89ef5fd68aa55e84f3379ff9b6")
            self.assertEqual(digest(target.read_bytes()), metadata["original_sha256"])
            setup.apply_gas_source_patch(self.source, spec)
        self.assertEqual(digest(target.read_bytes()), metadata["modified_sha256"])


if __name__ == "__main__":
    unittest.main()
