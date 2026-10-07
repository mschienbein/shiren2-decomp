#include "common.h"
typedef struct { char pad[0x8]; void *vtbl; } Obj8010C88C;
extern s32 D_80153AA0[];
void func_800AC68C(Obj8010C88C *);
/* Actor slot +0xC supplies signed deletion flags. */
void func_8010C88C(Obj8010C88C *self, s32 flags) {
    self->vtbl = D_80153AA0;
    if (flags & 1) {
        func_800AC68C(self);
    }
}
