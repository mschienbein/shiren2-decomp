#include "common.h"
typedef struct VTable VTable;
/* Base-object prefix through the genuine vtable pointer at +0x24. */
typedef struct { unsigned char pad0[0x24]; VTable *vtable; } Object;
extern VTable D_8015BEF8;
extern void func_800EFD28(void *, s32);
extern void func_800A3918(void *);
void func_8010636C(Object *object, s32 flags) {
    object->vtable = &D_8015BEF8;
    func_800EFD28(object, 0);
    if (flags & 1) func_800A3918(object);
}
