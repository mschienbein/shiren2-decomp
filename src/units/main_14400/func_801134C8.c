#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
extern u8 D_80148644[4];

void func_801134C8(void) {
    u8 *p = D_80148644;
    s32 i = 3;

    do {
        *p++ = 0;
    } while (i-- > 0);
}
