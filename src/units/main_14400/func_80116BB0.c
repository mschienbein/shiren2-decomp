#include "common.h"
typedef struct { unsigned char pad_00[0xC]; unsigned char field_0C; } Object;
void func_80116BB0(Object *self) { self->field_0C |= 0x10; }
