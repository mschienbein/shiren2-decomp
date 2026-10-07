#include "common.h"

typedef unsigned char u8;

extern u8 D_80160B08[];
extern u8 D_00194FC0[];
extern u8 D_2000630[];
extern void func_8006AC30(void *arg0, void *arg1, void *arg2, s32 arg3, s32 arg4, s32 arg5);

/* receiver: unused; callers (0x8010947C, 0x8010964C, 0x80109854) pass the object pointer in a0. */
void *func_80044DCC(void *receiver, u8 index) {
    func_8006AC30(D_80160B08, D_00194FC0, D_2000630, 8, index - 1, 1);
    return D_80160B08;
}
