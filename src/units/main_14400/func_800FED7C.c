#include "common.h"

typedef unsigned char u8;

typedef struct {
    char pad0[0x24];
    void *vtable;
    char pad28[0xA0 - 0x28];
} Obj800EFC70;

extern char D_8015AB80[];

Obj800EFC70 *func_800EFC70(Obj800EFC70 *obj, s32 arg1, u8 arg2);

/* Derived constructor: base init with kind 0x2F, then install this class's vtable. */
Obj800EFC70 *func_800FED7C(Obj800EFC70 *obj, u8 id) {
    func_800EFC70(obj, 0x2F, id);
    obj->vtable = D_8015AB80;
    return obj;
}
