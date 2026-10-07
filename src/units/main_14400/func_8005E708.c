#include "common.h"

/* Word-wide input: subtraction wraps modulo 2^32 before the unsigned test.
 * Observed callers pass byte loads, but the predicate preserves every input bit.
 * Original type names, API meaning and translation-unit grouping are unknown. */
s32 func_8005E708(u32 value)
{
    return (value - 0x30U) < 10U;
}
