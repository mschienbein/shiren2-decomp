#include "common.h"

typedef unsigned char u8;

/* Partial view: base object, derived vtable pointer at 0x24. */
typedef struct Obj8010573C {
    unsigned char pad_00[0x24];
    void *vtable_24;
} Obj8010573C;

/* Initialized original vtable, not BSS. Its full type is unresolved. */
extern unsigned char D_8015BD78[];

void *func_800EFC70(void *obj, s32 arg1, u8 arg2);

/* Constructor: base constructor with kind 0x47, then the derived vtable. */
Obj8010573C *func_8010573C(Obj8010573C *obj, u8 arg)
{
    func_800EFC70(obj, 0x47, arg);
    obj->vtable_24 = D_8015BD78;
    return obj;
}
