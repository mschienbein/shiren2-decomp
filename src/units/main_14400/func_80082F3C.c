#include "common.h"
typedef unsigned char u8;
extern s32 D_8013E8E4;
extern u8 *D_801A9050[];
static inline void set_nibble(u8 *dest, s32 index, s32 value) {
    if (value) {
        if (index & 1) *dest = value | (*dest & 0xF0);
        else *dest = (*dest & 0xF) | (value << 4);
    }
}
void func_80082F3C(s32 arg0, u8 *arg1) {
    s32 i = 0;
    u8 *dest = D_801A9050[D_8013E8E4] + arg0 / 2;
    do {
        u8 value = *arg1;
        s32 high = value >> 4;
        u32 low = value & 15;
        set_nibble(dest, arg0, high);
        arg0++;
        if (!(arg0 & 1)) dest++;
        set_nibble(dest, arg0, low);
        arg0++;
        if (!(arg0 & 1)) dest++;
        i += 2;
        arg1++;
    } while (i < 16);
}
