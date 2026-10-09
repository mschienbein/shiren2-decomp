#include "common.h"

typedef unsigned char u8;

typedef struct { signed char x, y; } Pair;
typedef struct { s32 handle; Pair position; } Playback;
typedef struct { short type; Playback playback; } State;
extern State D_80161650;
void *func_80052888(void) { return &D_80161650; }
