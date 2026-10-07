#include "common.h"

typedef unsigned char u8;
typedef signed short s16;

typedef struct { u8 pad0[0x18]; s16 x18; s16 pad1A; void (*x1C)(void *, s32, void *); } VTable;
typedef struct { u8 pad0[0x18]; VTable *x18; } Obj;
extern u8 D_80153A44[];
void func_800CA4A4(Obj *obj, void *type);
void func_800AF11C(u8 *arg, Obj *obj) {
    func_800CA4A4(obj, D_80153A44);
    obj->x18->x1C((u8 *)obj + obj->x18->x18, 1, arg + 2);
}
