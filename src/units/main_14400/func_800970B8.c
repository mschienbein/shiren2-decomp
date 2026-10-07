#include "common.h"
typedef struct { char pad[0x4C]; void *vtbl; } Obj800970B8;
extern s32 D_80151E38[];
void func_800D8FA8(void *object);
void func_800970B8(Obj800970B8 *self, u32 flags) {
    self->vtbl = D_80151E38;
    if (flags & 1) {
        func_800D8FA8(self);
    }
}
