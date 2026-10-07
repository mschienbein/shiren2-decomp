#include "common.h"

typedef struct { char pad[0xF]; unsigned char count; unsigned char items[16]; } S;
s32 func_8010BC2C(S *s, unsigned char i) {
    if (i >= s->count) return 0;
    return s->items[i];
}
