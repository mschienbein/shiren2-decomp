#include "common.h"
typedef struct { signed char x, y; } Pair;
typedef struct { s32 handle; Pair position; } Playback;
typedef struct { short type; Playback playback; } State;
extern State D_80161650;
extern void func_80052B50(s32);
extern void func_800533DC(void);
void func_8005239C(void) { func_80052B50(D_80161650.playback.handle); func_800533DC(); }
