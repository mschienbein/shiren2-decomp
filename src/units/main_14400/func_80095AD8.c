#include "common.h"
extern void *D_80140234[8];
extern s32 D_80140254;
extern void func_80041434(s32), func_80045A24(s32), func_80095D64(void *), func_80046A40(s32), func_80046A94(s32, s32), func_80095D04(void *);
/* func_80046CB0 keeps its second argument in s3 and stores the selected
 * result word or a sentinel (0x80000001) through it: an output pointer.
 */
extern s32 func_80046CB0(void *, s32 *out);
s32 func_80095AD8(void *a, s32 *out, s32 c) { s32 result; D_80140234[0] = a; D_80140254 = 0; if (c) func_80041434(1); func_80045A24(3); func_80095D64(a); func_80046A40(0x10); result = func_80046CB0(a, out) != 0; func_80045A24(3); func_80046A94(0x10, 0); func_80095D04(a); if (c) func_80041434(0); return result; }
