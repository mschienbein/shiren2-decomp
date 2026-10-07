#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;

extern u16 D_801486E8[24];
extern u8 D_80148718;
extern u8 D_80148719;
u16 *func_80116720(void) {
    u8 prev = D_80148718;
    if (++D_80148718 >= 24) {
        D_80148718 = 0;
    }
    if (D_80148719 == D_80148718) {
        D_80148718 = prev;
        return 0;
    }
    return &D_801486E8[D_80148718];
}
