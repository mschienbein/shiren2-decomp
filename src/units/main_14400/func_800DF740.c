#include "common.h"

typedef short s16;

typedef struct VtEntry {
    s16 delta;
    s16 index;
    void *pfn;
} VtEntry;

typedef struct Obj800DF740 {
    s16 kind_00;
    const VtEntry *vtable_04;
    s32 field_08;
} Obj800DF740;

extern const VtEntry D_80157FA8[];
extern const VtEntry D_80158AA8[];

/* 12-byte object built by func_80092... factory: base vtable, kind 0x2E, then derived vtable. */
Obj800DF740 *func_800DF740(Obj800DF740 *obj) {
    obj->vtable_04 = D_80157FA8;
    obj->kind_00 = 0x2E;
    obj->vtable_04 = D_80158AA8;
    obj->field_08 = 1;
    return obj;
}
