#include "common.h"

typedef signed short s16;
typedef struct {
    s16 field00, field02;
    float field04, field08, field0C, field10, field14, field18, field1C, field20, field24, field28, field2C;
    s16 field30, pad32; s32 field34; s16 field38, pad3A; s32 field3C;
    s16 field40, pad42; s32 field44, field48;
} Object;

void func_80063F40(Object *self) {
    self->field00 = 0;
    self->field02 = 0;
    self->field30 = 0;
    self->field34 = 0;
    self->field38 = 0;
    self->field3C = 0;
    self->field40 = 0;
    self->field44 = 0;
    self->field48 = 0;
    self->field18 = self->field1C = self->field20 = self->field0C =
        self->field10 = self->field14 = self->field08 = self->field04 = 0.0f;
    self->field24 = self->field28 = self->field2C = 1.0f;
}
