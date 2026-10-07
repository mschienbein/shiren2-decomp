#include "common.h"
extern s32 D_80151F30[];
extern s32 D_80151E38[];
void func_800D8FA8(void *object);
typedef struct { char pad0[0x4C]; void *vtbl4C; char pad50[0xB0 - 0x50]; void *vtblB0; } Obj;
void func_80096B84(Obj *self, s32 flags) {
    self->vtbl4C = D_80151F30;
    self->vtblB0 = D_80151E38;
    self->vtbl4C = D_80151E38;
    if (flags & 1) {
        func_800D8FA8(self);
    }
}
