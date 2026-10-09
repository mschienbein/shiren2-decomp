#include "common.h"
/* Complete 16-byte layout record (.data 0x80139084..0x80139093, {2, 0x18, 0xA, 8}): func_80095064
 * forwards it to func_800950A8, whose func_800486A4 reads +0, +4, +8 and +0xC. */
typedef struct { s32 columns, width, x, y; } Layout80096140;
extern Layout80096140 D_80139084;
void func_80095064(void *, s32, void *);
void func_8009ACE0(char *p) { func_80095064(p + 0x5C, 0x29B, &D_80139084); }
