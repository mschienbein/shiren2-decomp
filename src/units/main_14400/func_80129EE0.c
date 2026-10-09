#include "common.h"
extern s32 func_8012C4B0(void *resource);
extern void func_8012A870(s32 handle);
s32 func_80129EE0(void *resource) {
    s32 handle = func_8012C4B0(resource);
    func_8012A870(handle);
    return handle;
}
