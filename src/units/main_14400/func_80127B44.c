#include "common.h"

typedef struct {
    s32 fields_00[2];
    const void *vtable;
} Obj80127B44;

extern const unsigned char D_80153AA0[];

void func_800AC68C(void *a);

void func_80127B44(Obj80127B44 *obj, s32 flags)
{
    obj->vtable = D_80153AA0;
    if (flags & 1) {
        func_800AC68C(obj);
    }
}
