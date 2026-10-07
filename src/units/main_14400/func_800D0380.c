#include "common.h"

/* Pair contents are opaque to construction; collection methods use an 8-byte stride. */
typedef struct Pair_800D0380 Pair_800D0380;

typedef struct Obj_800D0380 {
    unsigned char pad0[0x4];
    void *vtable;
    Pair_800D0380 *items;
    s32 capacity;
    s32 count;
} Obj_800D0380;

extern unsigned char D_801545E0[];
extern void func_800D03B4(Obj_800D0380 *obj, Pair_800D0380 *items, s32 capacity);

Obj_800D0380 *func_800D0380(Obj_800D0380 *obj, Pair_800D0380 *items, s32 capacity) {
    obj->vtable = D_801545E0;
    func_800D03B4(obj, items, capacity);
    return obj;
}
