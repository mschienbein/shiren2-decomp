#include "common.h"

typedef short s16;

typedef struct VtEntry {
    s16 delta;
    s16 index;
    void *pfn;
} VtEntry;

typedef struct Obj800CE5E4 {
    void *pool_00;
    const VtEntry *vtable_04;
} Obj800CE5E4;

extern const VtEntry D_80154300[];
void func_800D8FA8(void *object);

/* Destructor (vtable D_80154300 last slot): restore this class's vtable, free when flags bit 0 is set. */
void func_800CE5E4(Obj800CE5E4 *obj, s32 flags) {
    obj->vtable_04 = D_80154300;
    if (flags & 1) {
        func_800D8FA8(obj);
    }
}
