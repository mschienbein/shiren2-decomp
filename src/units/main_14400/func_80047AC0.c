#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

void func_80046374(void);
void func_80060C54(u32 mode);
void func_800B2B50(void);
extern s32 D_80138BF0;
void func_80047AC0(void *self) {
    func_80060C54(3);
    func_800B2B50();
    D_80138BF0 = 0;
    func_80046374();
}
