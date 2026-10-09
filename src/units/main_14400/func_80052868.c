#include "common.h"

typedef struct { signed char x, y; } Pair;
typedef struct { s32 handle; Pair position; } Playback;
/* Complete 0xC-byte sound state at D_8016165C (type +0, playback.handle +4,
 * playback.position +8/+9), the same layout as D_80161650 (func_800528BC
 * clears both records from one base each; func_80051E9C copies D_8016165C whole). */
typedef struct { short type; Playback playback; } State;
extern State D_8016165C;

/* Address of the third record's position pair (0x80161664 = D_8016165C + 8). */
Pair *func_80052868(void) {
    return &D_8016165C.playback.position;
}
