#include "common.h"

typedef struct { unsigned char pad00[0xA0]; s32 fieldA0; } S;
extern void func_800F6038(S *, s32);

void func_800F61D8(S *obj)
{
    obj->fieldA0 = 1;
    func_800F6038(obj, 2);
}
