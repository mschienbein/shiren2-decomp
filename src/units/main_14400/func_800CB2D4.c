#include "common.h"

typedef struct {
    unsigned char unk00[9];
    unsigned char unk09;
    unsigned char unk0A[0x1E];
    s32 unk28;
} Object;

/* Bit-mask tables in .rodata: D_8015488C[n] = 1 << n, D_80154894[n] = ~(1 << n). */
extern const unsigned char D_8015488C[8];
extern const unsigned char D_80154894[8];
extern void func_80048AD4(s32);

void func_800CB2D4(Object *object, s32 value) {
    switch (value ^ 1) {
    default:
        object->unk09 |= D_8015488C[1];
        break;
    case 0:
        object->unk09 &= D_80154894[1];
        break;
    }
    object->unk28 = value;
    func_80048AD4(value);
}
