#include "common.h"

typedef float f32;

/*
 * 2^x for pitch ratios: sixth-order Taylor series of e^(x ln 2) with
 * coefficients (ln 2)^n / n!; negative x evaluates 1 / 2^-x.
 */
f32 func_8012BBF4(f32 x) {
    f32 x2, x4;

    if (x == 0.0f) {
        return 1.0f;
    }
    if (x > 0.0f) {
        x2 = x * x;
        x4 = x2 * x2;
        return x * 0.693147180559945 + 1.0 + x2 * 0.240226506959101 + (x2 * x) * 0.0555041086648216 +
               x4 * 0.00961812910762848 + (x4 * x) * 0.00133335581464284 +
               (x4 * x2) * 0.000154035303933816;
    }
    x = -x;
    x2 = x * x;
    x4 = x2 * x2;
    return 1.0 / (x * 0.693147180559945 + 1.0 + x2 * 0.240226506959101 + (x2 * x) * 0.0555041086648216 +
                  x4 * 0.00961812910762848 + (x4 * x) * 0.00133335581464284 +
                  (x4 * x2) * 0.000154035303933816);
}
