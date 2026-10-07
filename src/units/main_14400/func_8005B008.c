#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

extern s32 D_80165394;
extern s32 D_80165400;
extern s32 D_80165408;
extern s32 D_801653F8;
void func_80059590(s32 arg0);
void func_8005B008(void) {
    func_80059590(1);
    D_80165394 = 4;
    D_80165400 = 0;
    D_80165408 = 0;
    D_801653F8 = 0;
}
