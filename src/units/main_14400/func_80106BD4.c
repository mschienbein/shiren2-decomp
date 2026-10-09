#include "common.h"

typedef unsigned char u8;

typedef struct {
    char pad00[0x24];
    const void *vtable;
} Obj800EFC70;

extern Obj800EFC70 *func_800EFC70(Obj800EFC70 *obj, s32 arg1, u8 arg2);
extern const unsigned char D_8015C080[200];

/* Constructor: base init with kind 0x4D, then install this class's vtable. */
Obj800EFC70 *func_80106BD4(Obj800EFC70 *obj, u8 arg) {
    func_800EFC70(obj, 0x4D, arg);
    obj->vtable = D_8015C080;
    return obj;
}
