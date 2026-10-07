#include "common.h"

typedef unsigned char u8;

typedef struct { s32 x, y; } Pair;
Pair *func_800CFBA4(Pair *dst, u8 *src) { dst->x = *(s32 *)(src + 8); dst->y = *(s32 *)(src + 0xC); return dst; }
