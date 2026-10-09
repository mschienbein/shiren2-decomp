#include "common.h"
typedef unsigned short u16;
typedef struct { char unk0[0xE4]; u16 unkE4; } State;
s32 func_800EE334(State *arg0) { return arg0->unkE4 & 1; }
