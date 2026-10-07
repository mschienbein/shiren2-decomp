#include "common.h"

void func_800A56D8(void *a, void *b, short c, unsigned short d);

s32 func_800A5758(void *a, void *b, short c, unsigned short d)
{
    if (b == 0) {
        return 0;
    }
    func_800A56D8(a, b, c, d);
    return 1;
}
