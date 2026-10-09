#include "common.h"
typedef struct { unsigned char pad_00[0xC]; unsigned char field_0C; } Child;
typedef struct { s32 field_00; Child *field_04; } Object;
extern void func_80113914(Child *obj);
void func_800A124C(Object *self) { func_80113914(self->field_04); }
