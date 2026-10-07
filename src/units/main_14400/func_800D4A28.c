#include "common.h"

typedef unsigned char u8;
extern char D_80143094[];
void func_800AFEB4(void *arg0, void *arg1, void *arg2, u8 arg3);

void func_800D4A28(void **arg0, s32 arg1, void *arg2) {
    if (*arg0 != 0) {
        func_800AFEB4(D_80143094, arg2, *arg0, arg1);
    }
}
