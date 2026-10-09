#include "common.h"
typedef unsigned short u16;
typedef struct { u16 field_00; } Object;
void func_800CABD8(Object *self) { if (self->field_00 < 0xFFFF) self->field_00++; }
