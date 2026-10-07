#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

/* v is the receiver object func_8010B87C later passes to func_8010B5D0. */
typedef struct { void *vt; void *v; } S;
extern u8 D_8015CC38[];
S *func_8010B864(S *s, void *v) { s->vt = D_8015CC38; s->v = v; return s; }
