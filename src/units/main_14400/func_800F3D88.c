#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { char pad0[0xA]; u8 fieldA; char padB[0x1D]; u16 field28, field2A, field2C, field2E, field30; u8 field32; char pad33[0x42]; u8 field75; char pad76[6]; u16 field7C, field7E; } Object;
typedef struct { u16 first_00; u8 count_02; u8 field_03; } Range;
typedef struct { u16 field0, field2, field4; char field6; u8 field7; } Entry;
Range *func_80045134(u8 kind);
Entry *func_800451A8(u8 kind, u8 variant);
void func_800E039C(Object *object, s32 value);
extern u32 D_8013960C;
void func_800F3D88(Object *obj, u8 level)
{
    Range *range = func_80045134(obj->fieldA);
    Entry *entry = func_800451A8(obj->fieldA, level);
    u8 clamped = level;
    if (range->count_02 < level) clamped = range->count_02;
    /* ODD_C: the clamped level is written back to the parameter before the two byte
       stores; storing `clamped` directly lets GCC drop the original register copy. */
    level = clamped;
    obj->field32 = level;
    obj->field75 = level;
    obj->field28 = entry->field0;
    obj->field2A = entry->field0;
    obj->field2C = entry->field2;
    obj->field2E = entry->field2;
    D_8013960C *= 2;
    obj->field30 = entry->field4;
    func_800E039C(obj, entry->field7);
    obj->field7C = 0;
    obj->field7E = 0;
    D_8013960C >>= 1;
}
