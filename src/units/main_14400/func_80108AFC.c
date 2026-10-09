#include "common.h"
typedef struct { unsigned char pad0[0xA0]; unsigned char field_A0; } Object;
s32 func_80108AFC(Object *self) { return self->field_A0 == 0; }
