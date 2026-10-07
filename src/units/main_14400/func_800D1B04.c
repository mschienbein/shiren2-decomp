#include "common.h"

extern char D_80140080[];
extern void func_80047A90(void *);
extern s32 func_80047AF4(void *, s32);
extern void func_80047AC0(void *);
s32 func_800D1B04(void) {
    s32 r;
    func_80047A90(D_80140080);
    r = func_80047AF4(D_80140080, 1);
    func_80047AC0(D_80140080);
    return r;
}
