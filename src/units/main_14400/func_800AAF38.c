#include "common.h"

typedef unsigned char u8;
void *func_800AAD88(u8, s32);
void *func_800AADB4(u8, s32);

void *func_800AAF38(void)
{
    void *object = func_800AAD88(3, 0);

    if (object == 0) {
        object = func_800AADB4(0x32, 0);
    }
    return object;
}
