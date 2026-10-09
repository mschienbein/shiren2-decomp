#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct VtEntry {
    s16 delta;
    s16 index;
    void *pfn;
} VtEntry;

typedef struct Obj800EFC70 {
    u8 pad_00[0x24];
    const VtEntry *vtable_24;
    u8 pad_28[0x9A - 0x28];
    u16 flags_9A;
} Obj800EFC70;

extern const VtEntry D_8015B2A8[];
Obj800EFC70 *func_800EFC70(Obj800EFC70 *obj, s32 arg1, u8 arg2);
void func_800E4D88(Obj800EFC70 *obj, s32 value);
void func_800E4D90(Obj800EFC70 *a, s32 b);

/* Constructor: base construct with kind 0x38, install vtable D_8015B2A8, set fields, flag bit 0. */
Obj800EFC70 *func_80100780(Obj800EFC70 *obj, u8 arg) {
    func_800EFC70(obj, 0x38, arg);
    obj->vtable_24 = D_8015B2A8;
    func_800E4D88(obj, 3);
    func_800E4D90(obj, 2);
    obj->flags_9A |= 1;
    return obj;
}
