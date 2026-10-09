#include "common.h"
s32 func_800ACEB4(void *self);
void func_800ACD34(void *self);
char *func_800AE674(void *self);
void func_800498E4(s32 id, ...);
s32 func_800A08D8(s32 mode, s32 key, s32 sel);
void func_800ACDD8(void *self) {
    if (func_800ACEB4(self) != 2) {
        func_800ACD34(self);
        func_800498E4(0x226, func_800AE674(self));
        func_800A08D8(1, -1, 0);
    }
}
