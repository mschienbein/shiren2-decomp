#include "common.h"

extern unsigned char D_801398B1;
extern void func_80053D70(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5,
                          char *fmt, void *args);

void func_80054524(char *fmt, ...) {
    func_80053D70(0, D_801398B1, 5, 0x17, 0x1E, 4, fmt, __builtin_next_arg(fmt));
}
