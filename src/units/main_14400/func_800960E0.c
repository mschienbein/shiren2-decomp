#include "common.h"
typedef struct { char pad[0x4C]; void *vtbl; } Obj;
extern s32 D_80151E38;
void func_800D8FA8(void *object);
void func_800960E0(Obj *self, s32 flags) {
    self->vtbl = &D_80151E38;
    if (flags & 1) func_800D8FA8(self);
}
