#include "common.h"
typedef unsigned short u16;
typedef struct { u16 unk0; u16 pad2; const void *vtbl; } Obj;
extern const s32 D_80157FA8[];
extern const unsigned char D_80158188[48];
Obj *func_800D9F48(Obj *self, unsigned char *unused_payload) {
    self->vtbl = &D_80157FA8;
    self->unk0 = 0x33;
    self->vtbl = D_80158188;
    return self;
}
