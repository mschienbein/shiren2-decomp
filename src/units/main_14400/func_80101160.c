#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    char pad0[0x24];
    void *vtable;
    char pad28[0x9A - 0x28];
    u16 flags9A;
    char pad9C[0xA0 - 0x9C];
} Obj800EFC70;

extern char D_8015B500[];

Obj800EFC70 *func_800EFC70(Obj800EFC70 *obj, s32 arg1, u8 arg2);
void func_800E4D88(Obj800EFC70 *obj, s32 value);
void func_800E4D90(Obj800EFC70 *obj, s32 value);

/* Derived constructor for kind 0x3B. */
Obj800EFC70 *func_80101160(Obj800EFC70 *obj, u8 id) {
    func_800EFC70(obj, 0x3B, id);
    obj->vtable = D_8015B500;
    func_800E4D88(obj, 4);
    func_800E4D90(obj, 2);
    obj->flags9A |= 1;
    return obj;
}
