#include "common.h"
void *func_800A7DEC(void *p);
s32 func_800A4CC4(void *p, void *v, void *a);
void func_800A4EC0(void *p, void *a);
s32 func_800A4EFC(void *p, void *a) {
    if (func_800A4CC4(p, func_800A7DEC(p), a)) {
        func_800A4EC0(p, a);
        return 1;
    }
    return 0;
}
