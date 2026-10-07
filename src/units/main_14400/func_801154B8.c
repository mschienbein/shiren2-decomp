#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;

typedef struct { u8 pad0[0x10]; s16 offset_10; s16 pad12; s32 (*fn_14)(void *self); } VTable801154B8;
typedef struct { s32 field_0; VTable801154B8 *vtable_4; } Sub801154B8;
typedef struct { u8 pad0[0xC]; Sub801154B8 sub_C; } Obj801154B8;

void func_801154B8(Obj801154B8 *obj) {
    Sub801154B8 *sub = &obj->sub_C;
    VTable801154B8 *vt = sub->vtable_4;
    vt->fn_14((u8 *)sub + vt->offset_10);
}
