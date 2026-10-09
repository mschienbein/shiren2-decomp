#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct {
    u8 pad0[0x10];
    s16 delta_10;
    s16 pad12;
    s32 (*func_14)(void *self);
} VTable800D8FF0;

typedef struct {
    u8 pad0[4];
    VTable800D8FF0 *vtable;
} Obj800D8FF0;

typedef struct Obj80094AFC Obj80094AFC;

extern Obj80094AFC D_80140160;

void func_80094AFC(Obj80094AFC *obj, void *msg);

s32 func_800D8FF0(Obj800D8FF0 *obj)
{
    func_80094AFC(&D_80140160, obj);
    return obj->vtable->func_14((u8 *)obj + obj->vtable->delta_10);
}
