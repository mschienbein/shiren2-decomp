#include "common.h"

typedef unsigned char u8;

typedef struct Target800DE644 {
    u8 kind_00;
} Target800DE644;

/* Partial view: only the target pointer at 0xC4 is used. */
typedef struct Obj800DE644 {
    u8 pad_00[0xC4];
    Target800DE644 *target_C4;
} Obj800DE644;

s32 func_800DDCA8(Obj800DE644 *obj);
s32 func_800AC670(void *obj);

/* Virtual method (vtable D_80158988): true when the object is in state 1 and its live
 * target is of kind 9. */
s32 func_800DE644(Obj800DE644 *obj)
{
    s32 ready = func_800DDCA8(obj) == 1;

    if (!ready) {
        return 0;
    }
    if (obj->target_C4 == 0) {
        return 0;
    }
    if (func_800AC670(obj->target_C4) != 0) {
        return 0;
    }
    return obj->target_C4->kind_00 == 9;
}
