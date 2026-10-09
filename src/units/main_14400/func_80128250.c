#include "common.h"

typedef struct {
    s32 field_0;
    s32 field_4;
    void *vtable;
} Obj;

extern unsigned char D_80153AA0[];
void func_800AC68C(void *a);

/* Destructor: restore the base vtable, free when bit 0 of the delete flags is set. */
void func_80128250(Obj *obj, s32 flags)
{
    obj->vtable = D_80153AA0;
    if (flags & 1) {
        func_800AC68C(obj);
    }
}
