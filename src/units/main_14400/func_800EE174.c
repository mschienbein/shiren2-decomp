#include "common.h"

typedef unsigned short u16;

void func_800EE174(unsigned char *obj)
{
    *(u16 *)(obj + 0xE4) |= 0x20;
}
