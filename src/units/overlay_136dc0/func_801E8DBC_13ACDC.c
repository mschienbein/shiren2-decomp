#include "func_801E8DBC_13ACDC/types.h"
extern s32 func_801E8AF4_13AA14(void);
extern void *func_800A8CB0(s32 cell);
extern s32 func_800A8974(void *obj);
void *func_801E8DBC_13ACDC(void) {
    void *obj = func_800A8CB0((u8)func_801E8AF4_13AA14());
    if (func_800A8974(obj)) return 0;
    return obj;
}
