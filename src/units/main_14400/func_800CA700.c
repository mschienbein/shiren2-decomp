#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x18];
    void *vtable18;
    /* Derived state initialized by CA600 at +0x1C/+0x20/+0x24. */
    u8 *buffer;
    u32 position;
    u32 capacity;
} Obj_800CA700;

extern char D_801541F8[];

Obj_800CA700 *func_800CA030(Obj_800CA700 *obj);
void func_800CA600(void *stream, u8 *buffer, u32 capacity);

/* Derived constructor: base init, install vtable, then initialize with both arguments. */
Obj_800CA700 *func_800CA700(Obj_800CA700 *obj, u8 *buffer, u32 capacity) {
    func_800CA030(obj);
    obj->vtable18 = D_801541F8;
    func_800CA600(obj, buffer, capacity);
    return obj;
}
