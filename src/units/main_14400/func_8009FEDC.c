#include "common.h"
typedef struct {
    unsigned char field_00[0x4c]; const void *field_4c;
    unsigned char field_50[0x10]; const void *field_60;
    unsigned char field_64[0xc]; s32 field_70;
} Object;
extern const unsigned char D_80153110[144], D_80151DF8[24];
extern Object *func_800953C0(Object *);
Object *func_8009FEDC(Object *self) {
    func_800953C0(self);
    self->field_4c = D_80153110;
    self->field_60 = D_80151DF8;
    self->field_70 = -1;
    return self;
}
