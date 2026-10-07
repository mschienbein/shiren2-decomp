#include "common.h"
typedef struct { unsigned char field_00[0x20]; short field_20; s32 (*field_24)(void *); } VTable;
typedef struct { s32 field_00; VTable *field_04; } Child;
typedef struct { unsigned char field_00, field_01; unsigned char field_02[0xAE]; Child field_B0; unsigned char field_B8[0xC]; void *field_C4; } Object;
extern char D_80143094[];
extern s32 func_800AFD08(void *, void *);
extern void func_800DDB64(Object *, unsigned char *, s32);
s32 func_800DE4A8(Object *object, unsigned char *data) { Child *child = &object->field_B0; s32 count = child->field_04->field_24((char *)child + child->field_04->field_20); *data++ = object->field_01; *data++ = count + 2; *data++ = func_800AFD08(D_80143094, object->field_C4); func_800DDB64(object, data, count); return count + 4; }
