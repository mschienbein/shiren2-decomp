#include "common.h"

typedef unsigned char u8;
typedef short s16;

/* Slot +0x94 targets func_800E115C / derived dispatchers; +0x90 is zero in their tables. */
typedef struct {
    u8 pad0[0x90];
    s16 delta_90;
    s16 pad92;
    s32 (*func_94)(void *self, s32 a, s32 b, u8 c, s32 d);
} VTable800E2870;

typedef struct {
    u8 pad0[0x24];
    VTable800E2870 *vtable_24;
} Obj800E2870;

void func_800E2870(Obj800E2870 *obj)
{
    obj->vtable_24->func_94((u8 *)obj + obj->vtable_24->delta_90, 2, 0x11, 0, 0);
}
