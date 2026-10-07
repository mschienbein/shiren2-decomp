#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
void func_80052260(s16);
/* The facade narrows the sound id to the delegate's signed halfword. */
void func_80045A24(s32 sound) { func_80052260((s16)sound); }
