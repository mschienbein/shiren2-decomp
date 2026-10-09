#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x90];
    short delta90;
    short pad92;
    s32 (*fn94)(void *self, s32 a1, s32 a2, u8 a3, s32 a4);
} VTable800E2AF4;

typedef struct {
    u8 pad0[0x24];
    VTable800E2AF4 *vtable;
} Obj800E2AF4;

s32 func_800E2AF4(Obj800E2AF4 *obj)
{
    VTable800E2AF4 *vt = obj->vtable;

    return vt->fn94((u8 *)obj + vt->delta90, 0, 13, 0xFE, 0);
}
