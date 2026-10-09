#include "common.h"

typedef unsigned char u8;
typedef struct VTable VTable;
typedef struct S {
    u8 pad_00[0x24];
    VTable *vtable_24;
    u8 pad_28[0x78];
    s32 field_A0;
} S;
typedef S Obj800EFC70;
extern Obj800EFC70 *func_800EFC70(Obj800EFC70 *object, s32 kind, u8 variant);
extern void func_8010523C(S *object);
extern VTable D_8015BCB8;

S *func_801051F0(S *object, u8 variant)
{
    func_800EFC70(object, 0x46, variant);
    object->vtable_24 = &D_8015BCB8;
    object->field_A0 = 1;
    func_8010523C(object);
    return object;
}
