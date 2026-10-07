#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;

s32 func_80058A20(s32 size);
void func_80131570(void *file, s32 channel);

void func_80058E58(void *owner) {
    func_80131570(owner, func_80058A20(0x10));
}
