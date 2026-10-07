#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

void func_800A3918(void *);
void func_800EFD28(void *, s32);
void func_801365F0(void *arg0, s32 arg1) {
    func_800EFD28(arg0, 0);
    if (arg1 & 1) {
        func_800A3918(arg0);
    }
}
