#include "common.h"

/* Original base vtable D_80153AA0 (rodata: adjustment words and function pointers),
 * modeled as one opaque whole object; only its address is stored here. */
typedef struct VTable VTable;

typedef struct {
    s32 fields_00[2];
    const VTable *vtable;
} Obj80118354;

extern const VTable D_80153AA0;

void func_800AC68C(void *a);

void func_80118354(Obj80118354 *obj, s32 flags)
{
    obj->vtable = &D_80153AA0;
    if (flags & 1) {
        func_800AC68C(obj);
    }
}
