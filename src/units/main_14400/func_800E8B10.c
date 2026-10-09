#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct {
    u8 pad0[0x90];
    s16 delta_90;
    s16 pad92;
    s32 (*func_94)(void *self, s32 a1, s32 a2, u8 a3, s32 a4);
    s16 delta_98;
    s16 pad9A;
    void *(*func_9C)(void *self);
} VTable800E8B10;

typedef struct {
    u8 pad0[0x24];
    VTable800E8B10 *vtable_24;
} Obj800E8B10;

s32 func_800E4454(Obj800E8B10 *obj);
s32 func_800CF250(void *target, u8 **out, s32 count);

u8 func_800E8B10(Obj800E8B10 *obj, u8 **out) {
    s32 ready = 0;
    s32 i;
    void *target;

    if (obj->vtable_24->func_94((u8 *)obj + obj->vtable_24->delta_90, 2, 9, 0, 0) == 0) {
        ready = func_800E4454(obj) == 0;
    }
    if (ready) {
        target = obj->vtable_24->func_9C((u8 *)obj + obj->vtable_24->delta_98);
        if (target != 0) {
            return func_800CF250(target, out, 2);
        }
    } else {
        for (i = 1; i != -1; i--) {
            out[i] = 0;
        }
    }
    return 0;
}
