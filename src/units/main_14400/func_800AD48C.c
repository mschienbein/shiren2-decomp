#include "common.h"
extern unsigned char D_80143044[32];
void func_800AD48C(void) {
    s32 i;
    for (i = 31; i != -1; --i) D_80143044[i] = 255;
}
