#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { s32 unk0; u8 unk4[0x20]; u8 unk24[0x20]; u8 unk44[0x20]; } Buf80046374;
extern s32 D_80138BF0;
extern Buf80046374 D_80138BF4;
s32 func_800A99D0(void);
void func_8005C59C(void *a, void *b, void *c, void *d);
void func_801E583C(s32 arg);
void func_80046374(void) {
    if (D_80138BF0 == 0 && func_800A99D0() != 0) {
        func_8005C59C(D_80138BF4.unk24, D_80138BF4.unk44, D_80138BF4.unk4, &D_80138BF4.unk0);
        D_80138BF0 = 1;
        func_801E583C(0);
        return;
    }
    if (D_80138BF0 == 0) {
        D_80138BF0 = 1;
    }
}
