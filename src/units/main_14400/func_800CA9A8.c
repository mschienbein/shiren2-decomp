#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 bytes[4]; } PackedWord;
typedef struct { u16 field_00; PackedWord field_02; u8 field_06, flags_07[3], field_0A, field_0B, field_0C, field_0D[5], pad_12[2]; u32 field_14; u32 field_18; void *field_1C; u8 pad_20[12]; u32 field_2C; } Object;
extern const u8 D_8015488C[8];
extern void func_800CA088(void *);
extern void func_800CA2C0(void *);
extern void func_800CB288(void *, s32);
extern void func_800CB2D4(void *, s32);
extern void func_800CB268(void *, s32);
extern void func_800CAD44(void *);
void func_800CA9A8(Object *self, u8 *data, s32 arg) {
    u8 *cursor;
    s32 count;
    func_800CA088(self->field_1C);
    func_800CA2C0(self->field_1C);
    cursor = self->flags_07;
    count = 2;
    do { *cursor++ = 0; } while (count-- > 0);
    self->flags_07[0] |= D_8015488C[0];
    func_800CB288(self, 0);
    func_800CB2D4(self, 1);
    func_800CB268(self, arg);
    self->field_00 = 0;
    self->field_0B = 0xB;
    self->field_0C = 0;
    self->field_14 = 0;
    cursor = self->field_0D;
    count = 4;
    do { *cursor++ = 0; } while (--count != -1);
    self->field_02 = *(PackedWord *)data;
    self->field_06 = 0;
    self->field_2C = 0;
    self->field_18 = 0;
    func_800CAD44(self);
}
