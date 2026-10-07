#include "common.h"

typedef unsigned char u8;
typedef signed short s16;

typedef struct { u8 pad0[0x18]; s16 x18; s16 pad1A; void (*x1C)(void *, s32, void *); } VTable;
typedef struct { u8 pad0[0x18]; VTable *vtable; } Obj;
extern u8 D_80153A38[];
extern u8 D_80148720[], D_801485F0[], D_80148650[], D_801485B0[], D_80148690[], D_80148470[];
void func_800CA4A4(Obj *obj, void *type);
void func_800AEF44(Obj *obj) {
    func_800CA4A4(obj, D_80153A38);
    obj->vtable->x1C((u8 *)obj + obj->vtable->x18, 0x16, D_80148720);
    obj->vtable->x1C((u8 *)obj + obj->vtable->x18, 0x1B, D_801485F0);
    obj->vtable->x1C((u8 *)obj + obj->vtable->x18, 0x13, D_80148650);
    obj->vtable->x1C((u8 *)obj + obj->vtable->x18, 0x13, D_801485B0);
    obj->vtable->x1C((u8 *)obj + obj->vtable->x18, 0xD, D_80148690);
    obj->vtable->x1C((u8 *)obj + obj->vtable->x18, 0x15, D_80148470);
}
