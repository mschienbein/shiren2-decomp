#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct { s32 unk0; s32 unk4; void *unk8; } S;
extern u8 D_80153AA0[];
void func_800AC68C(void *);
void func_8011E9C4(S *arg0, s32 arg1) {
    arg0->unk8 = D_80153AA0;
    if (arg1 & 1) {
        func_800AC68C(arg0);
    }
}
