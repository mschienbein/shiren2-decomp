#include "common.h"

typedef struct S {
    char pad0[0xA4];
    s32 fieldA4;
} S;

void func_800F6038(S *, s32);

void func_800F61FC(S *obj)
{
    obj->fieldA4 = 1;
    func_800F6038(obj, 2);
}
