#include "common.h"

typedef unsigned char u8;
typedef signed short s16;

typedef struct { u8 pad0[0x60]; s16 x60; s16 pad62; s32 (*x64)(void *, s32); } VTable;
typedef struct { u8 pad0[0x20]; s32 x20; u8 pad24[0x28]; VTable *x4C; } Obj;
typedef struct { s32 x0; s32 x4; } Arg;
void func_80095FD0(Obj *obj, Arg *arg) {
    obj->x4C->x64((u8 *)obj + obj->x4C->x60, arg->x0 + obj->x20 * arg->x4);
}
