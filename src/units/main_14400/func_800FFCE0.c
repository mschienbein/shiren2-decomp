#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x24];
    void *vtable_24;
} Obj800FFCE0;

/* Initialized original vtable; its full type is unresolved. */
extern unsigned char D_8015AFA8[];

Obj800FFCE0 *func_800EFC70(Obj800FFCE0 *obj, s32 arg1, u8 arg2);
void func_800FFD24(Obj800FFCE0 *obj);

/* Constructor: base init with kind 0x34, install vtable, finish setup. */
Obj800FFCE0 *func_800FFCE0(Obj800FFCE0 *obj, u8 id) {
    func_800EFC70(obj, 0x34, id);
    obj->vtable_24 = D_8015AFA8;
    func_800FFD24(obj);
    return obj;
}
