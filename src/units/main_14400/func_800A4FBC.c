#include "common.h"

extern void *func_800A7DEC(void *);
extern s32 func_800A4CC4(void *, void *, void *);
extern void func_800A4F58(void *, void *);
s32 func_800A4FBC(void *a, void *b) {
    if (func_800A4CC4(a, func_800A7DEC(a), b) != 0) {
        func_800A4F58(a, b);
        return 1;
    }
    return 0;
}
