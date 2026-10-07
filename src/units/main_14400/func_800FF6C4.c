#include "common.h"
typedef struct { char pad[0x24]; void *vtbl; } Obj800FF6C4;
extern s32 D_8015AE28[];
void func_800EFD28(Obj800FF6C4 *, s32);
void func_800A3918(Obj800FF6C4 *);
/* Unit slot +0xC supplies signed deletion flags. */
void func_800FF6C4(Obj800FF6C4 *self, s32 flags) {
    self->vtbl = D_8015AE28;
    func_800EFD28(self, 0);
    if (flags & 1) {
        func_800A3918(self);
    }
}
