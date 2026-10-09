#include "common.h"

typedef unsigned char u8;
typedef struct VTable VTable;
typedef struct { u8 pad_00[0x24]; VTable *field_24; } S;
typedef S Obj800A38A0;
extern VTable D_80149AF0;
extern void func_800EFD28(S *, s32);
extern void func_800A3918(Obj800A38A0 *obj);

void func_801366E0(S *object, s32 flags) {
    object->field_24 = &D_80149AF0;
    func_800EFD28(object, 0);
    if (flags & 1) {
        func_800A3918(object);
    }
}
