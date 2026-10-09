#include "common.h"
typedef struct { char pad[0x24]; const void *vtbl; } Obj;
extern const unsigned char D_8015AC40[192];
void func_800EFD28(Obj *, s32);
void func_800A3918(Obj *);
void func_800FEF8C(Obj *self, s32 flags) {
    self->vtbl = D_8015AC40;
    func_800EFD28(self, 0);
    if (flags & 1) func_800A3918(self);
}
