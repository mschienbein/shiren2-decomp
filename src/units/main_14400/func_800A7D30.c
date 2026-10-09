#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_0[0x1E]; u8 flags_1E; } Object;
s32 func_800A7D30(Object *p) { return (p->flags_1E >> 5) & 1; }
