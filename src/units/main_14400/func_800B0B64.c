#include "common.h"

typedef unsigned char u8;
typedef struct { u8 bits[5]; } BitSet;
/* 8-byte pool iterator (func_800B07E0/0808/0864): collection pointer at +0, cursor at +4. */
typedef struct { void *collection; unsigned short cursor; unsigned short pad6; } Iter;
typedef struct { u8 kind; char pad1[0x28]; u8 unk29; } Thing;
extern u8 *D_8015380C[];
extern u8 D_80153734[];
extern const u8 D_8015488C[8];
extern const u8 D_80154894[8];
extern u8 D_80143094[];
extern u8 D_8014313C[];
Iter *func_800B07E0(Iter *, void *);
s32 func_800B0808(Iter *);
Thing *func_800B0864(Iter *);
static inline void BitSet_init(BitSet *set) {
    u8 *p = set->bits;
    s32 n = 5;
    while (n-- > 0) {
        *p++ = 0;
    }
}
static inline void BitSet_add(u8 *bits, s32 n) {
    bits[n >> 3] |= D_8015488C[n & 7];
}
static inline s32 nonzero(s32 v) {
    return v != 0;
}
static inline s32 nonnegative(s32 v) {
    return v >= 0;
}
static inline s32 differs(s32 a, s32 b) {
    return a != b;
}
static inline s32 BitSet_has(u8 *bits, s32 n) {
    return (bits[n >> 3] & D_8015488C[n & 7]) != 0;
}
void func_800B0B64(void) {
    BitSet seen;
    Iter iter;
    u8 group;
    s32 i;
    u8 **table;
    BitSet_init(&seen);
    table = D_8015380C;
    for (group = 20; nonzero(group); group--) {
        u8 *list = table[group];
        if (list != 0) {
            for (i = D_80153734[group] - 1; nonnegative(i); i--) {
                u8 value = list[i];
                if (value != 0) {
                    BitSet_add(seen.bits, value - 1);
                }
            }
        }
    }
    func_800B07E0(&iter, D_80143094);
    while (func_800B0808(&iter)) {
        Thing *thing = func_800B0864(&iter);
        if (thing->kind == 9) {
            u8 value = thing->unk29;
            if (value != 0) {
                BitSet_add(seen.bits, value - 1);
            }
        }
    }
    for (i = 39; nonnegative(i); i--) {
        u8 *byte = &D_8014313C[i >> 3];
        s32 known = BitSet_has(D_8014313C, i);
        if (differs(known, BitSet_has(seen.bits, i)) && known) {
            *byte &= D_80154894[i & 7];
        }
    }
}
