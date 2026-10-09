#include "common.h"
extern const unsigned char D_80151DF8[24];
typedef struct { const void *vt; s32 pad[3]; s32 x10; } S;
S *func_800951AC(S *self) { self->vt = D_80151DF8; self->x10 = -1; return self; }
