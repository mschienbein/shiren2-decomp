#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

/* Complete 8-byte record (.bss 0x80160B28..0x80160B2F) filled by one 8-byte func_8006AC30
 * transfer (stride 8, count 1); D_00194FC0/D_2025528 are ROM base/table address symbols. */
extern u8 D_80160B28[8];
extern char D_00194FC0[];
extern char D_2025528[];
u16 *func_80044ECC(u8 arg0, u8 arg1);
void func_8006AC30(void *arg0, void *arg1, void *arg2, s32 arg3, s32 arg4, s32 arg5);
void *func_80044F5C(u8 arg0, u8 arg1) {
    u16 *entry = func_80044ECC(arg0, arg1);
    if (entry == 0) {
        return 0;
    }
    func_8006AC30(D_80160B28, D_00194FC0, D_2025528, 8, *entry + (arg1 - 1), 1);
    return D_80160B28;
}
