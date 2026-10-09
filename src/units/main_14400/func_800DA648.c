#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

extern const s32 D_80157FA8[];
extern const unsigned char D_80158358[48];
typedef struct { s16 x0; s16 pad; const void *vt; } S;
S *func_800DA648(S *self, unsigned char *unused_payload) { self->vt = &D_80157FA8; self->x0 = 4; self->vt = D_80158358; return self; }
