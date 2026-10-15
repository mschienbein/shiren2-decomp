#include "common.h"

typedef struct { s32 field_00, field_04; } Pair;
typedef struct { unsigned char value; } Dir;
extern Pair *func_800A2594(Pair *, void *, Dir);
extern void func_800A2758(Pair *, Dir);
extern s32 func_800B5900(void *, s32, s32, void **);
static inline Pair *copy_pair(Pair *out, const Pair *in) { out->field_00 = in->field_00; out->field_04 = in->field_04; return out; }
static inline Pair *make_pair(Pair *out, void *input, Dir direction) { func_800A2594(out, input, direction); return out; }
/* Main-approved explicit result buffer preserves the original o32 aggregate ABI.
 * The effect receiver occupies a1 but is unused by this implementation. */
Pair *func_8010ECD0(Pair *out, void *effect, Pair *input, Dir *direction) {
    Pair value, next;
    /* ORIGINAL BEHAVIOR (proposed exception): original sp+0x20 has no
     * caller initialization before B5900 at VRAM 0x8010ED20. Its pre-store
     * rejection branches at 0x800B5930/0x800B5998 reach the shared epilogue
     * at 0x800B59F8 and can leave the first hit unset.
     * Gameplay reachability of that first-probe path remains unproved. */
    void *hit;
    make_pair(&value, input, *direction);
    for (;;) {
        s32 done;
        copy_pair(&next, &value);
        done = func_800B5900(&next, 2, 2, &hit) ^ 1;
        if (done) break;
        func_800A2758(&value, *direction);
    }
    /* Original VRAM 0x8010ED48 reads sp+0x20 even after a first pre-store
     * return. A later pre-store return retains the previous return-1 null
     * write; an object rejection stores its pointer before returning. */
    if (!hit) {
        Dir reverse;
        reverse.value = (direction->value + 4) & 7;
        func_800A2758(&value, reverse);
    }
    return copy_pair(out, &value);
}
