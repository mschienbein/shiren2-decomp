#include "common.h"

typedef struct { signed char x, y; } Pair;
typedef struct { s32 handle; Pair position; } Playback;
/* Complete 0xC-byte sound state at D_80161644 (type +0, playback.handle +4,
 * playback.position +8/+9), the same layout as D_80161650 (func_800528BC
 * clears both records from one base each; func_80051E9C copies D_8016165C whole). */
typedef struct { short type; Playback playback; } State;
extern State D_80161644;
/* Returns the whole state record; func_80053070 stores it as void * (as func_80052888 does for D_80161650). */
void *func_80052878(void) {
    return &D_80161644;
}
