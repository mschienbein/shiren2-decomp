#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

extern s32 D_8013DE80;
extern s32 D_8013DE84;
extern s32 D_8013DE88;
extern s32 D_8013DE8C;
extern s32 D_8013DE90;
extern s32 D_8013DE94;
extern s32 D_8013DE9C;
extern s32 func_8007D388(void);
extern void func_80083568(void);

void func_8007D264(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    s32 before = func_8007D388();

    D_8013DE80 = arg0;
    D_8013DE84 = arg1;
    D_8013DE88 = arg2;
    D_8013DE8C = arg3;
    D_8013DE90 = arg4;
    D_8013DE94 = arg5;
    D_8013DE9C = arg6;
    if (before != func_8007D388()) {
        func_80083568();
    }
}
