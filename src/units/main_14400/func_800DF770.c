#include "common.h"

typedef unsigned short u16;

/* Inlined base/derived constructor pair: base vtable, kind, derived vtable, value. */
typedef struct Obj800DF770 {
    u16 kind_00;
    unsigned char pad_02[2];
    void *vtable_04;
    s32 value_08;
} Obj800DF770;

/* Initialized original vtables, not BSS. Their full type is unresolved. */
extern unsigned char D_80157FA8[];
extern unsigned char D_80158AA8[];

Obj800DF770 *func_800DF770(Obj800DF770 *obj, s32 value)
{
    obj->vtable_04 = D_80157FA8;
    obj->kind_00 = 0x2E;
    obj->vtable_04 = D_80158AA8;
    obj->value_08 = value;
    return obj;
}
