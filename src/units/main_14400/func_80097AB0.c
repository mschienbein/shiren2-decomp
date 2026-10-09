#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_00[0x4C]; const void *vtable_4C; } Base;
typedef struct { Base base; u8 pad_50[0x10]; const void *vtable_60; u8 pad_64[0xC]; s32 field_70; u8 pad_74[0xC]; } ChildC8;
typedef struct { Base base; u8 pad_50[0x18]; s32 field_68; const void *vtable_6C; u8 pad_70[0x9C]; } Child148;
typedef struct { Base base; u8 pad_50[0x10]; s32 field_60; const void *vtable_64; u8 pad_68[0x128]; s32 field_190; u8 pad_194[0xC]; s32 field_1A0; const void *vtable_1A4; } Child254;
typedef struct { Base base; u8 pad_50[0x78]; ChildC8 child_C8; Child148 child_148; Child254 child_254; } Object;
extern const u8 D_8014A9E8[], D_80152580[], D_80151E10[], D_80151EC8[], D_80152130[], D_80152098[];
extern const unsigned char D_80152AE8[144];
extern void *func_800953C0(void *self);
static inline void init_C8(ChildC8 *child) { func_800953C0(&child->base); child->base.vtable_4C = D_80152580; }
static inline void init_148(Child148 *child) { func_800953C0(&child->base); child->base.vtable_4C = D_80152AE8; }
static inline Child254 *init_254(Child254 *child) { func_800953C0(&child->base); child->base.vtable_4C = D_80152130; return child; }
void *func_80097AB0(Object *self) {
    Child254 *child;
    func_800953C0(&self->base);
    self->base.vtable_4C = D_8014A9E8;
    init_C8(&self->child_C8);
    self->child_C8.vtable_60 = D_80151E10;
    self->child_C8.field_70 = -1;
    init_148(&self->child_148);
    self->child_148.field_68 = -1;
    self->child_148.vtable_6C = D_80151EC8;
    child = init_254(&self->child_254);
    self->child_254.field_60 = -1;
    self->child_254.vtable_64 = D_80151EC8;
    child->base.vtable_4C = D_80152098;
    self->child_254.field_1A0 = -1;
    self->child_254.vtable_1A4 = D_80151EC8;
    self->child_254.field_190 = 0;
    return self;
}
