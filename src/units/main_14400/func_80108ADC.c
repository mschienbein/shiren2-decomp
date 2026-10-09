#include "common.h"
typedef struct { unsigned char pad0[0xA4]; s32 field_A4; } Object;
s32 func_80108ADC(Object *self, s32 value) { return self->field_A4 == value; }
