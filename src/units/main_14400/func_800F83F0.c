#include "common.h"
typedef struct { unsigned char field_00[0x1C]; unsigned short field_1C; unsigned char field_1E[6]; void *field_24; unsigned char field_28[0x4A]; unsigned char field_72; } Object;
extern char D_80159C10[];
extern void *func_800F3CF0(Object *, s32, unsigned char);
extern s32 func_800A3934(Object *);
extern void func_800E4D88(Object *, s32);
Object *func_800F83F0(Object *object, unsigned char kind) { func_800F3CF0(object, 0x5B, 1); object->field_24 = D_80159C10; if (!func_800A3934(object)) { func_800E4D88(object, 0); object->field_1C |= 1; object->field_72 &= 0xF7; } return object; }
