#include "common.h"
typedef struct { char pad[8]; void *vtbl; s32 unkC; s32 unk10; s32 unk14; } Obj8011BF90;
extern s32 D_8015E980[];
Obj8011BF90 *func_80112D20(Obj8011BF90 *, s32);
Obj8011BF90 *func_8011BF90(Obj8011BF90 *self) {
    func_80112D20(self, 0x27);
    self->vtbl = D_8015E980;
    self->unk10 = 0;
    self->unk14 = 0;
    return self;
}
