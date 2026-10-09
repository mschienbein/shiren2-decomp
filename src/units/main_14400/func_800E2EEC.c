#include "common.h"
typedef unsigned char u8;
typedef struct {
    u8 field_00[0x90]; short field_90; short field_92;
    s32 (*field_94)(void *, s32, s32, u8, s32);
} VTable;
typedef struct { u8 field_00[0x24]; VTable *field_24; } Object;
/* D_80158C98 + 0x94 resolves to func_800E115C. */
s32 func_800E2EEC(Object *object) {
    VTable *table = object->field_24;
    return table->field_94((u8 *)object + table->field_90, 0, 6, 0xFE, 0);
}
