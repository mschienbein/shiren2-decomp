#include "func_801E4FDC_133B2C/types.h"

void func_801E4EA0_1339F0(s32 kind, char *dst);
void func_801E58EC_13443C(void);
void func_801E5654_1341A4(s32 mode, char *ext);
s32 func_801E59A4_1344F4(void);

s32 func_801E4FDC_133B2C(s32 slot, s32 kind) {
    char ext[8];

    func_801E4EA0_1339F0(kind, ext);
    func_801E58EC_13443C();
    func_801E5654_1341A4(slot + 1, ext);
    return func_801E59A4_1344F4();
}
