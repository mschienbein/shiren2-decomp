#include "common.h"

typedef short s16;

/* g++ 2.x vtable entry: this-adjustment, index, function. */
typedef struct {
    s16 delta;
    s16 index;
    void *fn;
} VtableEntry;

typedef struct {
    s32 field_00;
    s32 field_04;
    VtableEntry *vtable;
} Obj801360A4;

extern VtableEntry D_80153AA0[];
extern void func_800AC68C(void *a);

/* Deleting destructor: restore the base vtable, free when bit 0 is set. */
void func_801360A4(Obj801360A4 *obj, s32 flags) {
    obj->vtable = D_80153AA0;
    if (flags & 1) {
        func_800AC68C(obj);
    }
}
