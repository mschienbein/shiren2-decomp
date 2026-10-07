#include "common.h"

extern void func_800F479C(void *, s32);
extern void func_800A3918(void *);
void func_8013638C(void *a, s32 flag) {
    func_800F479C(a, 0);
    if (flag & 1) func_800A3918(a);
}
