#include "common.h"
extern s32 func_8013085C(s32 arg0);
/* `offset` is supplied by the caller (func_801301E0 passes a live a1 at
 * 0x8013024C) but unused here. */
s32 func_801308B4(s32 arg, s32 offset) { return func_8013085C(arg) & ~0xF; }
