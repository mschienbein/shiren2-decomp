#include "common.h"
typedef struct { unsigned char field_00[0x24]; void *field_24; } Object;
extern s32 D_8015BFB8[];
extern void func_800EFD28(Object *obj, s32 flags);
extern void func_800A3918(Object *obj);
void func_80106768(Object *obj, s32 flags)
{
    obj->field_24 = D_8015BFB8;
    func_800EFD28(obj, 0);
    if (flags & 1) func_800A3918(obj);
}
