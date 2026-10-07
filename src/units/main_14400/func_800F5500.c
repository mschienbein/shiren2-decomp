#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { u8 pad0[0xA]; u8 field_A; u8 padB[0x14]; u8 field_1F; u8 pad20[4]; void *vtable; } Obj;
extern u8 D_80159760[];
Obj *func_800F4760(Obj *obj);
Obj *func_800F5500(Obj *obj) {
    Obj *self = obj;

    func_800F4760(obj);
    self->vtable = D_80159760;
    self->field_A = 0xB;
    self->field_1F = 0xB;
    return self;
}
