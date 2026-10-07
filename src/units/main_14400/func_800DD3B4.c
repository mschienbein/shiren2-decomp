#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct { s32 unk0; void *unk4; } S;
extern u8 D_80157FA8[];
void func_800D8FE8(void *);
void func_800DD3B4(S *arg0, s32 arg1) {
    arg0->unk4 = D_80157FA8;
    if (arg1 & 1) {
        func_800D8FE8(arg0);
    }
}
