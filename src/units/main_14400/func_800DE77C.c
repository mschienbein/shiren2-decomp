#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;

typedef struct { u8 pad0[0x20]; s16 offset_20; s16 pad22; s32 (*fn_24)(void *self); } VTable800DE77C;
typedef struct { s32 field_0; VTable800DE77C *vtable_4; } Sub800DE77C;
typedef struct { u8 field_0; u8 field_1; u8 pad2[0xAE]; Sub800DE77C sub_B0; } Obj800DE77C;
void func_800DDB64(Obj800DE77C *obj, u8 *out, s32 count);

s32 func_800DE77C(Obj800DE77C *obj, u8 *out) {
    Sub800DE77C *sub = &obj->sub_B0;
    VTable800DE77C *vt = sub->vtable_4;
    s32 count = vt->fn_24((u8 *)sub + vt->offset_20);

    *out++ = obj->field_1;
    *out++ = count + 1;
    func_800DDB64(obj, out, count);
    return count + 3;
}
