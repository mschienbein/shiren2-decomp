#include "common.h"
extern s32 D_80151F30[];
extern const unsigned char D_80151E38[144];
void func_800D8FA8(void *object);
typedef struct { char pad0[0x4C]; const void *vtbl4C; char pad50[0xB0 - 0x50]; const void *vtblB0; } Obj;
void func_80096B84(Obj *self, s32 flags) {
    self->vtbl4C = D_80151F30;
    self->vtblB0 = D_80151E38;
    self->vtbl4C = D_80151E38;
    if (flags & 1) {
        func_800D8FA8(self);
    }
}
