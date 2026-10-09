#include "common.h"
typedef unsigned char u8;
extern u8 D_8016166F, D_801397DA;
typedef struct { signed char x, y; } Pair;
typedef struct { s32 handle; Pair position; } Playback;
/* Complete 0xC-byte sound state at D_80161644 (type +0, playback.handle +4,
 * playback.position +8/+9), the same layout as D_80161650 (func_800528BC
 * clears both records from one base each; func_80051E9C copies D_8016165C whole). */
typedef struct { short type; Playback playback; } State;
extern State D_80161644;
/* Saved previous-stream state, the whole 0xC-byte record at 0x801397CC that
 * func_80051E9C block-copies D_8016165C into (lw/sw of three words from one
 * base). Its handle (+4) is a sequence ID from func_8012C330, not an address;
 * the original initializer is all zero. Defined here because this file's call
 * delay slot loads the handle through %lo. */
State D_801397CC = { 0 };
extern s32 func_8012A4E4(s32 id);
extern void func_800526B0(u8 a, u8 b);
void func_80052C90(void) {
    u8 value = 0x80;
    if (D_8016166F != 0 && func_8012A4E4(D_801397CC.playback.handle) == 0) {
        if (D_80161644.type == 0x3F) value = 0x60;
        func_800526B0(value, D_801397DA);
        D_8016166F = 0;
    }
}
