#include "common.h"

typedef unsigned char u8;

typedef struct { signed char x, y; } Pair;
typedef struct { s32 handle; Pair position; } Playback;
/* Complete 0xC-byte sound state at D_80161644 (type +0, playback.handle +4,
 * playback.position +8/+9), the same layout as D_80161650 (func_800528BC
 * clears both records from one base each; func_80051E9C copies D_8016165C whole). */
typedef struct { short type; Playback playback; } State;
extern State D_80161644;

s32 func_80052848(void) {
    return (u8)D_80161644.playback.position.x;
}
