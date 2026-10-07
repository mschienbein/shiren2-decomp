#include "common.h"
typedef unsigned char u8;
typedef struct { u32 romAddr; u8 unk4[0x30]; } TableEntry; /* romAddr: PI ROM address, not a CPU pointer */
extern TableEntry D_8013C09C[];
void func_8006AAF0(void *dst, u32 devAddr, s32 size);
s32 func_80066AD0(s32 index) {
    s32 result;
    if (index >= 16) index = 0;
    func_8006AAF0(&result, D_8013C09C[index].romAddr, 4);
    return result;
}
