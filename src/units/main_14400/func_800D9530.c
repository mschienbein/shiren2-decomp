#include "common.h"

typedef struct VTable VTable;
typedef struct {
    unsigned char pad0[4];
    VTable *field04;
    unsigned char pad08[0xBC];
    s32 fieldC4;
} Object;
extern VTable D_80158068;
extern Object *func_800DDAD0(Object *, s32);

Object *func_800D9530(Object *self, s32 value)
{
    Object *result = self;
    func_800DDAD0(self, 8);
    result->field04 = &D_80158068;
    result->fieldC4 = value;
    return result;
}
