#include "common.h"
/* Unit vtable at receiver+8: slot +0x4C u8 (void *self); slot +0x54 void (void *self, u8 kind). */
typedef struct {
    unsigned char field_00[0x48]; short field_48; unsigned char (*field_4c)(void *self);
    short field_50; void (*field_54)(void *self, unsigned char kind);
} Methods;
typedef struct {
    unsigned char field_00, field_01; unsigned char field_02[6]; Methods *field_08;
    unsigned char field_0c; signed char field_0d;
} Object;
typedef struct { s32 field_00; Object *field_04; } Wrapper;
/* RNG state object; only its address is passed here. */
extern unsigned char D_80147620[];
extern void func_800ACD34(Object *);
extern s32 func_800C5B58(void *, s32);
extern s32 func_8010BA90(Object *, signed short);
s32 func_800A1168(Wrapper *wrapper) {
    Object *self = wrapper->field_04;
    unsigned char kind;
    func_800ACD34(self);
    kind = self->field_08->field_4c((unsigned char *)self + self->field_08->field_48);
    if (kind != self->field_01) {
        self->field_08->field_54((unsigned char *)self + self->field_08->field_50, kind);
        return 2;
    }
    if (self->field_0d + 1 < 0x63 && func_800C5B58(D_80147620, 8)) {
        func_8010BA90(self, 3);
        return 1;
    }
    func_8010BA90(self, 1);
    return 0;
}
