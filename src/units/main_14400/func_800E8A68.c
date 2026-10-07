#include "common.h"

typedef unsigned char u8;
typedef signed short s16;

typedef struct {
    s16 delta;
    s16 index;
    void *func;
} VEntry800E8A68;

typedef struct {
    u8 pad0[0x90];
    VEntry800E8A68 query;
    VEntry800E8A68 target;
} VTable800E8A68;

typedef struct {
    u8 pad0[0x24];
    VTable800E8A68 *vtbl;
} Obj800E8A68;

typedef s32 (*QueryFunc800E8A68)(void *self, s32 arg1, s32 arg2, u8 arg3, s32 arg4);
/* Slot +0x98/+0x9C: item-container getter (targets func_800EE07C, func_800EB0B8). */
typedef u8 *(*TargetFunc800E8A68)(u8 *self);

s32 func_800E4454(Obj800E8A68 *obj);
void *func_800CF058(void *target, u8 arg1);

void *func_800E8A68(Obj800E8A68 *obj, u8 arg1) {
    s32 ready = 0;
    u8 *target;

    if (((QueryFunc800E8A68)obj->vtbl->query.func)((u8 *)obj + obj->vtbl->query.delta, 2, 9, 0, 0) == 0) {
        ready = func_800E4454(obj) == 0;
    }
    if (ready) {
        target = ((TargetFunc800E8A68)obj->vtbl->target.func)((u8 *)obj + obj->vtbl->target.delta);
        if (target != 0) {
            return func_800CF058(target, arg1);
        }
        return 0;
    }
    return 0;
}
