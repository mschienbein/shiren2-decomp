#include "common.h"

typedef unsigned char u8;

typedef struct { s32 unk0; u8 b4; } S;
s32 func_80083D40(s32);
/* Full-word shifted signed product; every caller narrows it to the low halfword. */
s32 func_80045608(S *s, s32 delta, s32 value) { return (value >> s->b4) * func_80083D40(delta); }
