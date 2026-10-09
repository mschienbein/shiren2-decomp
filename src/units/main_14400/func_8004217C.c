#include "common.h"
extern s32 func_800610A8(void);
extern s32 D_801F5CD4;
s32 func_8004217C(void) {
    if (func_800610A8()) return (unsigned char)D_801F5CD4;
    return 0;
}
