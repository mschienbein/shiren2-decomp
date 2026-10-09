#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct VtEntry {
    s16 delta;
    s16 index;
    void *pfn;
} VtEntry;

typedef struct Obj800EFC70 {
    u8 pad_00[0x24];
    const VtEntry *vtable_24;
} Obj800EFC70;

extern const VtEntry D_8015AA00[];
Obj800EFC70 *func_800EFC70(Obj800EFC70 *obj, s32 arg1, u8 arg2);

/* Constructor: base construct with kind 0x2D, then install vtable D_8015AA00. */
Obj800EFC70 *func_800FE2B0(Obj800EFC70 *obj, u8 arg) {
    func_800EFC70(obj, 0x2D, arg);
    obj->vtable_24 = D_8015AA00;
    return obj;
}
