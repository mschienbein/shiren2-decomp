#include "common.h"
extern s32 D_801616F4[]; /* Address-only view of the 0xC-byte channel. */
void func_80053590(void *p);
void func_800533DC(void) { func_80053590(&D_801616F4); }
