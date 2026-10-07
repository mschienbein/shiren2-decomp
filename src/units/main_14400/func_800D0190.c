#include "common.h"
typedef unsigned char u8; typedef unsigned short u16; typedef signed char s8; typedef short s16;
extern void func_800D01B8(void *output, void *helper, void *item);
void *func_800D0190(void *output, void *helper, void *item){ func_800D01B8(output, helper, item); return output; }
