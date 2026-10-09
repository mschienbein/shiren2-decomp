#include "common.h"

typedef short s16;

typedef struct VtEntry {
    s16 delta;
    s16 index;
    void *pfn;
} VtEntry;

typedef struct Obj80118798 {
    s32 field_00;
    s32 field_04;
    const VtEntry *vtable_08;
} Obj80118798;

extern const VtEntry D_80153AA0[];
void func_800AC68C(void *a);

/* Destructor (vtable slot at 0x8015E004): restore the base vtable D_80153AA0, free when flags bit 0 is set. */
void func_80118798(Obj80118798 *obj, s32 flags) {
    obj->vtable_08 = D_80153AA0;
    if (flags & 1) {
        func_800AC68C(obj);
    }
}
