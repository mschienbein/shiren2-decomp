#include "func_801EF770_141690/types.h"
extern const u8 D_80151E38[144];
extern void func_800D8FA8(void *object);

void func_801EF770_141690(S10Menu *self, s32 flags)
{
    self->vtable4C = D_80151E38;
    if (flags & 1) {
        func_800D8FA8(self);
    }
}
