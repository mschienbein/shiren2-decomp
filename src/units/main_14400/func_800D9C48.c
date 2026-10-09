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
    VtableEntry *vtable;
} Obj800D9C48;

extern VtableEntry D_80157FA8[];
extern void func_800D8FE8(void *object);

/* Deleting destructor: restore the base vtable, free when bit 0 is set. */
void func_800D9C48(Obj800D9C48 *obj, s32 flags) {
    obj->vtable = D_80157FA8;
    if (flags & 1) {
        func_800D8FE8(obj);
    }
}
