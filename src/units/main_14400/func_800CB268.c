#include "common.h"
typedef struct { unsigned char pad0[0xA]; unsigned char field_A; } Object;
void func_800499C0(s32 index);
void func_800CB268(Object *self, s32 value) {
    self->field_A = value;
    func_800499C0(value);
}
