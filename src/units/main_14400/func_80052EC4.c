#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

void *func_80053034(s16 key);
s32 func_80052EC4(s16 arg0) { return func_80053034(arg0) != 0; }
