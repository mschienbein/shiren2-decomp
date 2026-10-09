#include "common.h"
extern s32 func_8005B07C(void);
s32 func_80074114(void) {
    s32 value = func_8005B07C();
    if (value < 0) return 0;
    return value;
}
