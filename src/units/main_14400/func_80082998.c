#include "common.h"

extern unsigned short D_801A9F58;
extern unsigned short D_801A9F5A;
extern unsigned short D_801A9F5C;
extern unsigned short D_801A9F5E;

void func_80082998(void)
{
    unsigned short a = D_801A9F58;
    unsigned short b = D_801A9F5A;

    D_801A9F58 = 0;
    D_801A9F5E = b;
    D_801A9F5C = a;
    D_801A9F5A = b + 16;
}
