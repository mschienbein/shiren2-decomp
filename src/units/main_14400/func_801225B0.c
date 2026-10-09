#include "common.h"
typedef unsigned char u8;
typedef struct { char pad0[8]; u8 field8; } Obj80094DAC;
Obj80094DAC *func_800C9E10(void);
u8 func_800A9958(void);
/* Eight masks form one table at 8015488C..80154893. */
extern const u8 D_8015488C[8];
s32 func_801225B0(void) { s32 result = 0; if (!(func_800C9E10()->field8 & D_8015488C[2])) result = func_800A9958() == 4; return result; }
