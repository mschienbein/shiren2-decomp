#include "common.h"
typedef unsigned char u8;
extern s32 func_800D80B0(u8);
/* The only caller (func_800581E8) passes an un-narrowed int; the byte
 * narrowing (andi 0xFF) happens here, when forwarding to func_800D80B0. */
s32 func_80042AD8(s32 x) { return func_800D80B0(x); }
