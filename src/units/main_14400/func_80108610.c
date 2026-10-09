#include "common.h"
typedef struct {
    unsigned char field_00[0x24]; const void *field_24;
    unsigned char field_28[0x78]; unsigned char field_a0;
    s32 field_a4;
} Object;
extern const unsigned char D_8015C510[192];
extern Object *func_800EFC70(Object *, s32, unsigned char);
extern void func_800E4D88(Object *, s32);
Object *func_80108610(Object *self, s32 value) {
    func_800EFC70(self, 0x55, value & 0xff);
    self->field_24 = D_8015C510;
    self->field_a0 = 0;
    self->field_a4 = 0;
    func_800E4D88(self, 4);
    return self;
}
