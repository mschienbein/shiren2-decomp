#include "common.h"

typedef struct { signed char x, y; } Pair;
typedef struct { s32 handle; Pair position; } Playback;
typedef struct { short type; Playback playback; } State;
extern State D_80161650;
s32 func_8005278C(void) { return D_80161650.playback.handle; }
