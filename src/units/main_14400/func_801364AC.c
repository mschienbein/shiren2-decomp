#include "common.h"

void func_800F479C(void *, s32);
void func_800A3918(void *);

void func_801364AC(void *obj, s32 flags)
{
    func_800F479C(obj, 0);
    if (flags & 1) {
        func_800A3918(obj);
    }
}
