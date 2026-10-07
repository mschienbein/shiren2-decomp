#include "common.h"

extern s32 D_80148090;
void *func_800C5F60(void);

void func_800D739C(void *arg0) {
    if (arg0 == func_800C5F60()) {
        D_80148090 = 0;
    }
}
