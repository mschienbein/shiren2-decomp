#include "common.h"
typedef unsigned char u8; typedef unsigned short u16; typedef signed char s8; typedef short s16;
void func_8008B678(u8 *a){ *(s16*)(a+0xE)=1; *(s16*)(a+4)=4; }
