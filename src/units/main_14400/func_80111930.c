#include "common.h"
typedef struct { unsigned char field_00[12]; unsigned char field_0c; } Object;
extern unsigned short D_801576C0[];
extern unsigned char D_801576FC[];
extern unsigned char func_800AE98C(Object *);
extern s32 func_800AC584(unsigned short);
s32 func_80111930(Object *self) {
    s32 kind = func_800AE98C(self) & 0xff;
    s32 base = func_800AC584(D_801576C0[kind]);
    return base + ((u32)(base * D_801576FC[kind] * self->field_0c) / 100);
}
