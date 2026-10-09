#include "common.h"
typedef unsigned char u8;
extern u8 D_801480DC[21], D_80148190[162], D_80148100[142], D_801480F4[9];
void func_800D7710(void) {
    u8 *p;
    s32 n;
    p=D_801480DC; n=20; do { *p++=0; } while (n-- > 0);
    p=D_80148190; n=161; do { *p++=0; } while (n-- > 0);
    p=D_80148100; n=141; do { *p++=0; } while (n-- > 0);
    p=D_801480F4; n=8; do { *p++=0; } while (n-- > 0);
}
