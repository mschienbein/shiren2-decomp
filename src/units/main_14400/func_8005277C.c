#include "common.h"
typedef struct { signed char x, y; } Pair;
typedef struct { s32 handle; Pair position; } Playback;
/* Complete 0xC-byte sound state at D_8016165C (type +0, playback.handle +4,
 * playback.position +8/+9), the same layout as D_80161650 (func_800528BC
 * clears both records from one base each; func_80051E9C copies D_8016165C whole). */
typedef struct { short type; Playback playback; } State;
extern State D_8016165C;
s32 func_8005277C(void) { return D_8016165C.type; }
