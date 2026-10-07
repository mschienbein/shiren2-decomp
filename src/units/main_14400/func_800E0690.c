#include "common.h"

extern s32 func_800A99D0(void);
extern unsigned short func_800E08B0(void *obj);
extern unsigned short func_800E08F0(void *obj);
extern s32 func_800E0534(void *obj, s32 amount);
extern s32 func_800E07A8(void *obj, s32 amount);
extern void func_800E075C(void *obj);

short func_800E0690(void *obj, short arg1, short arg2) {
    s32 useFirst = 0;
    short result;

    if (func_800A99D0() != 0 || arg1 < 0 || func_800E08B0(obj) < func_800E08F0(obj)) {
        useFirst = 1;
    }
    if (useFirst) {
        result = (short)func_800E0534(obj, arg1);
    } else {
        result = (short)func_800E07A8(obj, arg2);
        func_800E075C(obj);
    }
    return result;
}
