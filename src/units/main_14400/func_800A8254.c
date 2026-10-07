#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

void func_800A8254(u8 *a) { *(u16 *)(a + 0x1C) |= 0x10; }
