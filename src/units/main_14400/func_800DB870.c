#include "common.h"
typedef struct { s32 unk0; void *vtbl; } Obj;
extern s32 D_801584D8;
void *func_800DA8A0(void *obj, s32 kind, void *src);
Obj *func_800DB870(Obj *self, void *src) {
    func_800DA8A0(self, 0x12, src);
    self->vtbl = &D_801584D8;
    return self;
}
