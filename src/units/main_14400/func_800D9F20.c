#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { s16 f_0; s16 pad; const void *vt; } S;
extern u8 D_80157FA8[];
extern const unsigned char D_80158188[48];
S *func_800D9F20(S *s) { s->vt = D_80157FA8; s->f_0 = 0x33; s->vt = D_80158188; return s; }
