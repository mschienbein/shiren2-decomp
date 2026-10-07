#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad0[2]; u16 field_2; u8 pad4[0x28]; } Entry80083920;
extern Entry80083920 D_801A9080[];
s32 func_80083920(void) {
    s32 i;
    for (i = 0; i < 10; i++) {
        if (D_801A9080[i].field_2 != 0) {
            return 1;
        }
    }
    return 0;
}
