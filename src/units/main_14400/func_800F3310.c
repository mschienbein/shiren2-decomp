#include "common.h"
s32 func_800E1CD4(void *obj, s32 flag);
s32 func_800E7104(void *obj);
s32 func_800E8350(void *obj);
s32 func_800F3310(void *obj) {
    s32 result;
    if (func_800E1CD4(obj, 0x10) == 0) result = func_800E7104(obj);
    else result = func_800E8350(obj);
    return result;
}
