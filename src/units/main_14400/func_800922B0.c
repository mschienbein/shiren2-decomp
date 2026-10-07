#include "common.h"
extern void func_80092284(void *);
extern void func_800D8FA8(void *);
void func_800922B0(void *object, s32 flags) {
    func_80092284(object);
    if (!(flags & 1)) return;
    func_800D8FA8(object);
}
