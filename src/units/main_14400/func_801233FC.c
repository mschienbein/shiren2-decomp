#include "common.h"
typedef struct { char pad[0x8]; const void *vtbl; } Obj;
extern const s32 D_80153AA0[];
void func_800AC68C(Obj *);
void func_801233FC(Obj *self, s32 flags) {
    self->vtbl = &D_80153AA0;
    if (flags & 1) func_800AC68C(self);
}
