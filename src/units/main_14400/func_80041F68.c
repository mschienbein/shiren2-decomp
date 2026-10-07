#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;

extern s32 func_800610A8(void);
extern u8 D_801F5228[];
u8 func_80041F68(s32 id){ if (!func_800610A8()) return 0; return D_801F5228[(u8)id];}
