#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x24];
    void *vtable;
} Obj800FBD90;

extern u8 D_8015A3A8[];

void *func_800A38FC(s32 size);
Obj800FBD90 *func_800EFC70(Obj800FBD90 *obj, s32 arg1, u8 arg2);
void func_800E4D88(Obj800FBD90 *obj, s32 value);

/* Inline copy of the class constructor (its out-of-line copy is func_800FC3EC). */
static inline Obj800FBD90 *construct(Obj800FBD90 *obj, u8 level)
{
    func_800EFC70(obj, 0x24, level);
    obj->vtable = D_8015A3A8;
    func_800E4D88(obj, 1);
    return obj;
}

/* Monster factory (D_8015CC64 entry): construct in `place`, or in a new 0xA0-byte block. */
Obj800FBD90 *func_800FBD90(u8 level, Obj800FBD90 *place)
{
    if (place) {
        return construct(place, level);
    } else {
        return construct(func_800A38FC(0xA0), level);
    }
}
