#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef struct { signed char x, y; } Pair;
typedef struct { s32 handle; Pair position; } Playback;
/* Complete 0xC-byte sound state at D_80161644 (type +0, playback.handle +4,
 * playback.position +8/+9), the same layout as D_80161650 (func_800528BC
 * clears both records from one base each; func_80051E9C copies D_8016165C whole). */
typedef struct { short type; Playback playback; } State;
extern u8 D_8016166E;
extern State D_80161644;
extern u8 D_8014B894[];
void func_80051D14(s16);
s32 func_8012A534(s32 id, s32 value);
void func_8005312C(s32, u8);

void func_80052598(s16 index, u8 arg)
{
    if (D_8016166E == 0) {
        func_80051D14(index);
        func_8012A534(D_80161644.playback.handle, 0x1E);
        {
            s32 sound = D_8014B894[index];

            D_80161644.playback.position.x = 0x1E;
            func_8005312C(sound, arg);
        }
    }
}
