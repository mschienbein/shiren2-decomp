#include "common.h"

typedef short s16;
typedef unsigned short u16;
typedef double f64;

/*
 * Compute the signed 16.16 per-tick velocity that moves a 16-bit value from
 * vol to tgt over count / 8 ticks (consumed by func_8012EA08).  The
 * integral part is returned (floored) and the 16-bit fraction is stored
 * through ratel.  count == 0 yields the saturated velocity.
 */
s16 func_8012E8B0(f64 vol, f64 tgt, s32 count, u16 *ratel)
{
    f64 inverse;
    f64 rate;
    f64 shifted;
    f64 scaled;
    s16 integral;
    s16 carry;

    if (count == 0) {
        if (vol <= tgt) {
            *ratel = 0xFFFF;
            return 0x7FFF;
        }
        *ratel = 0;
        return -0x8000;
    }
    inverse = 1.0 / count;
    if (tgt < 1.0) {
        tgt = 1.0;
    }
    if (vol <= 0.0) {
        vol = 1.0;
    }
    rate = (tgt - vol) * inverse * 8.0;

    /* floor(rate): truncate, then shift the remainder into (0, 2) so a
     * negative remainder borrows one from the integral part. */
    integral = rate;
    shifted = rate - integral + 1.0;
    carry = shifted;
    scaled = (shifted - carry) * 65535.0;
    integral--;
    integral += carry;
    *ratel = (u32)scaled;
    return integral;
}
