#include "common.h"

typedef unsigned short u16;
typedef struct { char pad[0xE4]; u16 fE4; } S;
s32 func_800EE2C4(S *a){return (a->fE4>>3)&1;}
