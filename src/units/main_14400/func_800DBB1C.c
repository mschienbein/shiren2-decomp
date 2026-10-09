#include "common.h"
typedef struct { s32 unk0; const void *vtbl; } Obj800DBB1C;
extern const unsigned char D_80158508[48];
void *func_800DA904(void *obj, s32 kind, unsigned char *params);
Obj800DBB1C *func_800DBB1C(Obj800DBB1C *self, unsigned char *arg1) {
    func_800DA904(self, 0x13, arg1);
    self->vtbl = D_80158508;
    return self;
}
