#include "common.h"
typedef struct { s32 unk0; const void *vtbl; } Obj;
extern const unsigned char D_801584D8[48];
void *func_800DA8A0(void *obj, s32 kind, void *src);
Obj *func_800DB870(Obj *self, void *src) {
    func_800DA8A0(self, 0x12, src);
    self->vtbl = D_801584D8;
    return self;
}
