#include "common.h"
extern s32 D_8015AA00[];
void func_800EFD28(void *, s32);
void func_800A3918(void *);
typedef struct { char pad0[0x24]; void *vtbl; } Obj;
void func_800FE340(Obj *self, s32 flags) {
    self->vtbl = D_8015AA00;
    func_800EFD28(self, 0);
    if (flags & 1) {
        func_800A3918(self);
    }
}
