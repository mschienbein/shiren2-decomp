#include "common.h"

typedef unsigned char u8;

void *func_800C5F60(void);
void func_800A665C(void *arg0, void *arg1);
s32 func_800DA560(u8 *obj) {
    func_800A665C(func_800C5F60(), obj + 8);
    return 1;
}
