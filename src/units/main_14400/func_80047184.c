#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

typedef struct {
    u8 kind;
    u8 pad1[0xB];
    u8 flagsC;
    u8 padD[2];
    s8 unkF;
} Item80047184;

typedef struct {
    u8 pad0[0x2EC];
    s32 (*filter)(Item80047184 *item);
} Obj80047184;

Item80047184 *func_800980F0(Obj80047184 *self, s32 arg);
s32 func_80099FFC(Obj80047184 *self, s32 arg);
s32 func_800ACEB4(Item80047184 *item);
char *func_800ACAEC(Item80047184 *item);

s32 func_80047184(Obj80047184 *self, s32 arg) {
    Item80047184 *item = func_800980F0(self, arg);
    s32 bonus;
    u8 kind;
    s32 isTwo;
    s32 value;

    if (item == 0) {
        return 0x3C << 25;
    }
    if (self->filter != 0) {
        s32 failed = self->filter(item) != 1;
        if (failed) {
            return 0x39 << 25;
        }
    }
    bonus = 0;
    if (func_80099FFC(self, arg)) {
        bonus = -2;
    }
    kind = item->kind;
    isTwo = func_800ACEB4(item) == 2;
    value = isTwo ? 0x3C : 0x1C;
    value += bonus;
    if (!isTwo && func_800ACAEC(item)) {
        value = bonus + 0x14;
    } else if ((u8)(kind - 3) < 2) {
        if (isTwo && item->unkF != 0) {
            value = bonus + 0x14;
        }
    } else if (kind == 2) {
        if (item->flagsC & 1) {
            value = bonus + 0x1C;
        }
    }
    return value << 25;
}
