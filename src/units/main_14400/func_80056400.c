#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct {
    u8 pad0[0x44];
    s32 field_44;
    u8 pad48[0x5C - 0x48];
    s16 field_5C;
    u8 pad5E[0x60 - 0x5E];
} Entry80056400;

extern Entry80056400 D_801D40DC[];
extern s32 D_8013A260;
void func_80056504(s32 index);

void func_80056400(void) {
    s32 i;

    for (i = 0; i < 32; i++) {
        if (D_801D40DC[i].field_44 != -1 && D_801D40DC[i].field_5C == 0) {
            func_80056504(i);
        }
    }
    D_8013A260 = 1;
}
