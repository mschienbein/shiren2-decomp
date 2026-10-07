#include "common.h"

s32 func_800D1604(void *obj, s32 index);

s32 func_800D15B0(void *obj) {
    s32 i = 0;

    do {
        if (func_800D1604(obj, i) != 0) {
            return 1;
        }
        i++;
    } while (i < 5);
    return 0;
}
