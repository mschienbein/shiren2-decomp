#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

/* Slot +0x64 targets (func_800E32E8, func_800EA254, func_800ECC74, func_800F2720,
 * func_800F4688, func_800F62F0) produce no result. */
typedef struct VTable800E5E4C {
    u8 pad_00[0x60];
    s16 adjust_60;
    s16 pad_62;
    void (*method_64)(void *self);
} VTable800E5E4C;

typedef struct Unit800E5E4C {
    u8 pad_00[0xA];
    u8 field_0A;
    u8 pad_0B[0x1F - 0xB];
    u8 field_1F;
    u8 pad_20[0x24 - 0x20];
    VTable800E5E4C *vtable_24;
    u8 pad_28[0x35 - 0x28];
    u8 slots_35[10];
    u8 pad_3F;
    u16 field_40;
    u8 field_42;
    u8 field_43;
    u8 field_44;
    u8 pad_45[0x50 - 0x45];
    u16 field_50;
    u8 pad_52[0x75 - 0x52];
    u8 field_75;
} Unit800E5E4C;

extern u16 D_80158C6C[];
extern u16 D_8014767C;
s32 func_800E04D0(Unit800E5E4C *obj);
s32 func_800E0F40(Unit800E5E4C *obj);
s32 func_80049CB4(s32 id, ...);
extern void func_800A7C1C(Unit800E5E4C *obj);

s32 func_800E5E4C(Unit800E5E4C *unit, s32 notify) {
    s32 i;

    for (i = 9; i != -1; i--) {
        unit->slots_35[i] = 0;
    }
    unit->field_42 = 0;
    unit->field_43 = 0;
    unit->field_50 = D_80158C6C[(u8)func_800E04D0(unit)];
    unit->field_44 = 0;
    unit->field_40 = 0;
    if (notify != 0) {
        func_80049CB4(0xA7, unit);
    }
    unit->field_1F = unit->field_0A;
    unit->field_75 = func_800E0F40(unit);
    if (notify != 0 && (D_8014767C & 3)) {
        func_800A7C1C(unit);
    }
    unit->vtable_24->method_64((u8 *)unit + unit->vtable_24->adjust_60);
    return 1;
}
