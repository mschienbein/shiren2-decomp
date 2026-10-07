#include "common.h"
typedef unsigned char u8; typedef unsigned short u16; typedef signed char s8; typedef short s16;
typedef struct { u8 x0; u8 pad[7]; } E; extern E D_80156CD5[]; extern u8 D_80142F24; extern s32 func_800A99A8(void);
u8 func_800A9958(void){ u8 r; if (func_800A99A8()==0) r = D_80142F24; else r = D_80156CD5[D_80142F24-12].x0; return r; }
