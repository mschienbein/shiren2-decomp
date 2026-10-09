#include "common.h"
typedef struct { unsigned char pad_00[0x72]; unsigned char field_72; } Object;
void func_800E2650(Object *self) { self->field_72 |= 2; }
