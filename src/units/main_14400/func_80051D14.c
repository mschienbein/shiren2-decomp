#include "common.h"
typedef short s16;
typedef unsigned char u8;
typedef struct { u8 unk0, unk1; } Pair;
typedef struct { signed char x, y; } Position;
typedef struct { s32 handle; Position position; } Playback;
/* Complete 0xC-byte sound state at D_80161644 (type +0, playback.handle +4,
 * playback.position +8/+9), the same layout as D_80161650 (func_800528BC
 * clears both records from one base each; func_80051E9C copies D_8016165C whole). */
typedef struct { short type; Playback playback; } State;
extern const u8 D_8014B894[];
extern State D_80161644;
extern void func_80052944(s16);
extern void func_80051D94(s16, Pair);
void func_80051D14(s16 arg0) {
    Pair pair;
    if (arg0 == -1) D_80161644.type = arg0;
    else if (arg0 < 0x54) func_80052944(arg0);
    else {
        pair.unk0 = D_8014B894[arg0];
        pair.unk1 = 0x80;
        func_80051D94(arg0, pair);
    }
}
