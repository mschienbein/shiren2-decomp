#include "common.h"
s32 func_80049CB4(s32 id, ...);
/* Slot +0x14 of mode vtable D_8014A7A8 (0x8014A7BC). Dispatchers func_800AABEC
 * (0x800AAC00..0x800AAC10) and func_800B2B50 (0x800B2B90..0x800B2B9C) pass the adjusted
 * receiver and discard v0, like the void targets func_80042D84 and func_80043228 in the same
 * slot. The receiver is supplied but unused here. */
void func_800C25A8(void *self) { func_80049CB4(5, 1); }
