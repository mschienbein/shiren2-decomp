#include "common.h"
typedef unsigned short u16;
extern char D_80147620[];
extern s32 func_800B68B0(void *);
extern void *func_800A33DC(void *, void *);
extern u16 func_800C58DC(void *, u16);
extern void *func_800B6A98(void *, void *, s32);
void *func_800B6BA0(void *out, void *in) { s32 index = func_800B68B0(in); if (!index) { func_800A33DC(out, in); } else { u16 value = func_800C58DC(D_80147620, index - 1); func_800B6A98(out, in, value); } return out; }
