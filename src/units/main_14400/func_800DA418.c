#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { s16 x0; s16 pad; void *x4; } S;
extern u8 D_80157FA8[];
extern u8 D_801582A8[];
S *func_800DA418(S *s, unsigned char *unused_payload) {
    s->x4 = D_80157FA8;
    s->x0 = 0x39;
    s->x4 = D_801582A8;
    return s;
}
