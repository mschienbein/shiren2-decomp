#include "common.h"

typedef short s16;
typedef struct { s32 x; s32 y; } Pair;

Pair *func_800C5314(Pair *out, unsigned char *src)
{
    s32 x = *(s16 *)(src + 0xE);
    s32 y = *(s16 *)(src + 0xC);

    out->x = x;
    out->y = y;
    return out;
}
