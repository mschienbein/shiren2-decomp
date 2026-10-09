#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { signed char x, y; } Pair;
typedef struct { s32 handle; Pair position; } Playback;
typedef struct { short type; Playback playback; } State;
extern State D_80161650;
s32 func_8012A4E4(s32 id);
s32 func_800524F4(void) {
    return func_8012A4E4(D_80161650.playback.handle);
}
