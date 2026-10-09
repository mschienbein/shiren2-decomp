#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef struct { s16 n; s16 pad; void *p; } S;
extern u8 D_80157FA8[];
extern u8 D_80158038[];
S *func_800D94C8(S *s, unsigned char *unused_payload) { s->p = D_80157FA8; s->n = 7; s->p = D_80158038; return s; }
