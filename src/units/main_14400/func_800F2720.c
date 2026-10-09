#include "common.h"
/* Slot +0x90/+0x94 targets func_800F212C and its u8-argument overrides. */
typedef struct { unsigned char field_00[0x90]; short adjust; short field_92; s32 (*call)(void *, s32, s32, unsigned char, s32); } VTable;
typedef struct { unsigned char field_00[0x20]; u32 field_20; VTable *field_24; unsigned char field_28[0x68]; u32 field_90; } Object;
extern s32 func_800E1CD4(Object *obj, s32 value);
extern u32 D_80148270;
void func_800F2720(Object *obj)
{
    if (obj->field_24->call((unsigned char *)obj + obj->field_24->adjust, 2, 9, 0, 0)) {
        obj->field_20 = 0;
    } else {
        obj->field_20 = obj->field_90;
        if (func_800E1CD4(obj, 15)) obj->field_20 &= ~D_80148270;
    }
}
