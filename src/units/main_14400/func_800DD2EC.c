#include "common.h"

extern unsigned char D_80157FA8[];
void func_800D8FE8(void *);

void func_800DD2EC(void **obj, s32 flags)
{
    obj[1] = D_80157FA8;
    if (flags & 1) {
        func_800D8FE8(obj);
    }
}
