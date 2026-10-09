#include "common.h"
typedef struct { short field_0, field_2; unsigned char field_4[0xAC]; } Object;
extern Object D_801D2C2C[], D_801DEAB4[];
extern void func_80074778(Object *);
static inline Object *indexed(Object *base, s32 index) { return &base[index]; }
void func_80074B90(s32 index) { Object *self = indexed(D_801D2C2C, index); if (self->field_2 != -1) func_80074778(self); func_80074778(&D_801DEAB4[index]); }
