#include "common.h"
typedef unsigned char u8; typedef unsigned short u16; typedef signed char s8; typedef short s16;
extern s32 func_800610A8(void); extern u8 func_801E976C(void*);
u8 func_8004252C(void *a){ if (!func_800610A8()) return 0; return func_801E976C(a); }
