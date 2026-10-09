#include "common.h"

extern const unsigned char D_80151E38[144];
void func_800D8FA8(void *object);
typedef struct { char pad[0x4C]; const void *vtbl; } S;
void func_8009FD4C(S *self, s32 flags) { self->vtbl = D_80151E38; if (flags & 1) func_800D8FA8(self); }
