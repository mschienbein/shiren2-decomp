#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

extern s32 D_80157FA8;
extern s32 D_80158358;
typedef struct { s16 x0; s16 pad; void *vt; } S;
S *func_800DA648(S *self) { self->vt = &D_80157FA8; self->x0 = 4; self->vt = &D_80158358; return self; }
