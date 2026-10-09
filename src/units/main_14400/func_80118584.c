#include "common.h"

/* Original base vtable D_80153AA0 (rodata: adjustment words and function pointers),
 * modeled as one opaque whole object; only its address is stored here. */
typedef struct VTable VTable;

typedef struct {
    s32 field_0;
    s32 field_4;
    const VTable *vtable;
} Obj;

extern const VTable D_80153AA0;
void func_800AC68C(void *a);

/* Destructor: restore the base vtable, free when bit 0 of the delete flags is set. */
void func_80118584(Obj *obj, s32 flags)
{
    obj->vtable = &D_80153AA0;
    if (flags & 1) {
        func_800AC68C(obj);
    }
}
