#include "common.h"

extern s32 func_800E4454(void *obj);
extern void func_800E4470(void *obj);
extern s32 func_800F212C(void *obj, s32 arg1, s32 arg2, unsigned char arg3, s32 arg4);

s32 func_800FF544(void *obj, s32 arg1, s32 arg2, unsigned char arg3, s32 arg4) {
    if (arg1 == 0 && arg2 == 2 && func_800E4454(obj)) {
        func_800E4470(obj);
    }
    return func_800F212C(obj, arg1, arg2, arg3, arg4);
}
