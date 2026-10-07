#include "common.h"
extern s32 D_80151DF8;
typedef struct { void *vt; s32 pad[3]; s32 x10; } S;
S *func_800951AC(S *self) { self->vt = &D_80151DF8; self->x10 = -1; return self; }
