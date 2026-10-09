#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_0[0x90]; short offset_90; short pad_92; s32 (*method_94)(void *, s32, s32, u8, s32); } VTable;
typedef struct { u8 pad_0[0x1E]; u8 field_1E; u8 pad_1F[5]; VTable *field_24; } Obj;
extern u32 D_8013960C;
/* Trap apply slot +0x54 supplies five pointers; only the target object is used here. */
void func_8011FB88(void *unused, void *source, Obj *obj, void *direction, void *attacker) {
    if (obj->field_1E & 0x7C) {
        D_8013960C <<= 1;
        obj->field_24->method_94((u8 *)obj + obj->field_24->offset_90, 0, 0x12, 0xFE, 1);
        D_8013960C >>= 1;
    }
}
