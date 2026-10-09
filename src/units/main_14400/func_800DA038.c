#include "common.h"
typedef unsigned short u16;
typedef struct { u16 unk0; u16 pad2; const void *vtbl; } Obj800DA038;
extern s32 D_80157FA8[];
extern const unsigned char D_801581B8[48];
Obj800DA038 *func_800DA038(Obj800DA038 *self, unsigned char *unused_payload) {
    self->vtbl = D_80157FA8;
    self->unk0 = 0x34;
    self->vtbl = D_801581B8;
    return self;
}
