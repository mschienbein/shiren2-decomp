#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_0[0xD]; u8 flags_D; } Object;
s32 func_80128EF8(Object *p) { return p->flags_D & 0x7F; }
