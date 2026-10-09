#include "common.h"
typedef unsigned short u16;
typedef struct { char unk0[0x9A]; u16 unk9A; } State;
void func_800F3B90(State *arg0) { arg0->unk9A |= 2; }
