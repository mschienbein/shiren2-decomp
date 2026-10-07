#include "common.h"
void func_800F479C(void *p, s32 v);
void func_800A3918(void *p);
void func_80136344(void *p, s32 flags) {
    func_800F479C(p, 0);
    if (flags & 1) func_800A3918(p);
}
