#include "common.h"

typedef struct { s32 field_00, field_04; } Pair;
typedef struct { unsigned char value; } Dir;
extern Pair *func_800A2594(Pair *, void *, Dir);
extern void func_800A2758(Pair *, Dir);
extern s32 func_800B5900(Pair *, s32, s32, s32 *);
static inline Pair *copy_pair(Pair *out, const Pair *in) { out->field_00 = in->field_00; out->field_04 = in->field_04; return out; }
static inline Pair *make_pair(Pair *out, void *input, Dir direction) { func_800A2594(out, input, direction); return out; }
/* Main-approved explicit result buffer preserves the original o32 aggregate ABI.
 * The effect receiver occupies a1 but is unused by this implementation. */
Pair *func_8010ECD0(Pair *out, void *effect, Pair *input, Dir *direction) {
    Pair value, next;
    s32 hit;
    make_pair(&value, input, *direction);
    for (;;) {
        s32 done;
        copy_pair(&next, &value);
        done = func_800B5900(&next, 2, 2, &hit) ^ 1;
        if (done) break;
        func_800A2758(&value, *direction);
    }
    if (!hit) {
        Dir reverse;
        reverse.value = (direction->value + 4) & 7;
        func_800A2758(&value, reverse);
    }
    return copy_pair(out, &value);
}
