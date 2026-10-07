#include "common.h"
typedef unsigned short u16;
typedef struct { u16 unk0; u16 pad2; void *vtbl; } Obj;
extern s32 D_80157FA8;
extern s32 D_80158188;
Obj *func_800D9F48(Obj *self) {
    self->vtbl = &D_80157FA8;
    self->unk0 = 0x33;
    self->vtbl = &D_80158188;
    return self;
}
