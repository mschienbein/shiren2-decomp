#include "common.h"
void *func_800B4D80(void *pos);
/* Item-family slot +0x3C (root table D_80154300): void *(self, u32 index). This
 * single-entry set (table D_801544C0) yields its position-backed entry for index 0. */
void *func_800CFC2C(char *p, u32 index) { void *r; if (index) r = 0; else r = func_800B4D80(p + 8); return r; }
