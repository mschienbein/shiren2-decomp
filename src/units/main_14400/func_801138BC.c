#include "common.h"

typedef unsigned char u8;
extern u8 D_80156939;

void func_801138BC(u8 *obj)
{
    if (obj[0xC] == 0) {
        obj[0xC] = D_80156939;
    }
}
