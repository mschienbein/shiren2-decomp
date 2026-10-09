#include "common.h"
typedef struct { unsigned char field_00[0x24]; const void *field_24; } Object;
extern const unsigned char D_8015C208[192];
extern void func_800EFD28(Object *, s32);
extern void func_800A3918(Object *);
void func_80107318(Object *self, s32 flags) {
    self->field_24 = D_8015C208;
    func_800EFD28(self, 0);
    if (flags & 1) func_800A3918(self);
}
