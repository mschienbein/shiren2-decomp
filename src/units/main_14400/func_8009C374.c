#include "common.h"

void func_8009543C(void *, void *);
typedef struct { char pad[0x50]; short x50; char *x54; s32 x58; char pad2[0x18]; s32 x74; } S;
void func_8009C374(S *self, char *text, void *layout) { func_8009543C(self, layout); self->x50 = 0; self->x54 = text; self->x58 = 0; self->x74 = 1; }
