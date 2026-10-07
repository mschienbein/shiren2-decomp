#include "common.h"
typedef unsigned char u8; typedef unsigned short u16; typedef signed char s8; typedef short s16;
extern u8 D_80157FA8[]; extern u8 D_80158358[];
typedef struct { s16 x0; s16 x2; void *x4; } S;
S *func_800DA620(S *s){ s->x4=D_80157FA8; s->x0=4; s->x4=D_80158358; return s; }
