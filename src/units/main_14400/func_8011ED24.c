#include "common.h"
typedef unsigned char u8; typedef unsigned short u16; typedef signed char s8; typedef short s16;
typedef struct { u8 pad[0x1C]; void *vt; u8 pad2[0x18]; } L;
extern u8 D_8015F0E0[]; extern s32 func_80111A20(void*, u8*); extern L *func_80111E08(L*, void*, void*, void*, u8*); extern void func_800C2D0C(L*);
void func_8011ED24(void *a, u8 *b){ L l; if (func_80111A20(a, b)) { L *lp = &l; func_80111E08(lp, b, a, b, b+8); lp->vt = D_8015F0E0; func_800C2D0C(lp); } }
