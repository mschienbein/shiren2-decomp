#include "common.h"

typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

typedef struct { u8 pad0[0x18]; s16 x18; s16 pad1A; void (*x1C)(void *, s32); } VTable;
typedef struct { s32 x0; VTable *x4; } Sub;
typedef struct { u8 pad0[6]; u16 x6; } Info;
typedef struct {
    u8 pad0[0x28]; u16 x28; u16 x2A; u8 pad2C[6]; u8 x32; u8 pad33[0x45]; s32 x78;
    u8 pad7C[0x3C]; s16 xB8; u8 padBA[0x12]; Sub xCC;
} Obj;
Info *func_80044E7C(Obj *obj);
void func_8010B380(Obj *obj, u8 kind) {
    Sub *sub = &obj->xCC;
    Info *info;
    sub->x4->x1C((u8 *)sub + sub->x4->x18, 10);
    info = func_80044E7C(obj);
    obj->x32 = 1;
    obj->x78 = 0;
    obj->x28 = info->x6;
    obj->x2A = info->x6;
    obj->xB8 = 0x17C;
}
