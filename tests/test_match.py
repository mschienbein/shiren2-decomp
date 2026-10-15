from __future__ import annotations

import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
from match import check, padding_overrun, resolve_targets, segment_link_rules  # noqa: E402
from rom import CANONICAL_ROM, PROJECT  # noqa: E402

KMC_RANGE = "gcc272-kmc26-mips2-unsigned"


class PaddingOverrunTests(unittest.TestCase):
    CODE = b"\x01" * 420

    def test_zero_padding_past_the_slot_is_refused(self) -> None:
        self.assertEqual(padding_overrun(self.CODE + bytes(12), 420, 420), 12)

    def test_padding_inside_a_joint_slot_is_owned(self) -> None:
        self.assertEqual(padding_overrun(self.CODE + bytes(12), 420, 432), 0)
        self.assertEqual(padding_overrun(self.CODE + bytes(12), 420, 440), 0)

    def test_code_overflow_and_nonzero_tails_are_not_padding(self) -> None:
        self.assertEqual(padding_overrun(self.CODE + bytes(12), 424, 420), 0)
        self.assertEqual(padding_overrun(self.CODE + bytes(8) + b"\x01" * 4, 420, 420), 0)

    def test_unpadded_text_has_no_overrun(self) -> None:
        self.assertEqual(padding_overrun(self.CODE, 420, 420), 0)


class SegmentLinkRuleTests(unittest.TestCase):
    def test_text_uses_the_build_segment_subalign(self) -> None:
        self.assertEqual(segment_link_rules(0x105090)[:2], ("main_14400", 1))  # func_80132050
        # Resident `main` is `subalign: 1` by reviewed configuration (merge-011 H12; docs/OMP_WORKFLOW.md),
        # no longer the splat default SUBALIGN(16).
        self.assertEqual(segment_link_rules(0x1400)[:2], ("main", 1))


@unittest.skipUnless(
    (PROJECT / ".cache/gcc-kmc/cc1").is_file()
    and (PROJECT / ".cache/binutils-kmc-2.6/as").is_file()
    and (PROJECT / ".cache/binutils-gnu-2.9.1-fp32/as").is_file()
    and (PROJECT / "build/latest.json").is_file()
    and CANONICAL_ROM.is_file(),
    "Pinned GCC 2.7.2, KMC gas 2.6, FP32 GNU gas, an accepted build and the ROM required",
)
class KmcRangeCheckTests(unittest.TestCase):
    SOURCE = PROJECT / "src/units/main_14400/func_80132C5C.c"

    def run_check(self, profile_id: str):
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        out = Path(temporary.name) / "out"
        result, _diff = check(resolve_targets(["func_80132C5C"]), self.SOURCE, profile_id, out)
        return result, (out / "link.ld").read_text()

    def test_kmc_split_unit_is_exact_but_its_padding_is_refused(self) -> None:
        result, link = self.run_check(KMC_RANGE)
        self.assertIn(".text 0x80132C5C : SUBALIGN(1) { candidate.o(.text) }", link)
        self.assertEqual((result.text_bytes, result.slot_bytes, result.differing_words), (432, 420, 0))
        self.assertEqual(result.status, "policy")
        self.assertEqual(len(result.problems), 1)
        self.assertIn("padded to 432 bytes", result.problems[0])
        self.assertIn("12 byte(s) past the natural slot end 0x80132E00", result.problems[0])

    def test_bound_split_unit_still_matches(self) -> None:
        result, link = self.run_check("gcc272-gnu291-mips2-unsigned")
        self.assertIn(".text 0x80132C5C : SUBALIGN(1) { candidate.o(.text) }", link)
        self.assertEqual((result.status, result.text_bytes, result.problems), ("match", 420, []))


if __name__ == "__main__":
    unittest.main()
