#include "func_801E5060_133BB0/types.h"

s32 func_801E53BC_133F0C(s32 index);
u32 func_801E5788_1342D8(void);
void func_801E4EA0_1339F0(s32 kind, char *dst);
void func_801E58EC_13443C(void);
void func_801E52E0_133E30(s32 unused, char *ext);
s32 func_801E59A4_1344F4(void);

s32 func_801E5060_133BB0(s32 slot, s32 kind) {
    char ext[8];

    if (D_801E7830_136380[slot] == 0xFF) {
        return -1;
    }
    func_801E53BC_133F0C(D_801E7830_136380[slot]);
    if (func_801E5788_1342D8() == 0) {
        func_801E4EA0_1339F0(kind, ext);
        func_801E58EC_13443C();
        func_801E52E0_133E30(1, ext);
    }
    return func_801E59A4_1344F4();
}
