#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { u8 pad0[8]; void *vtable; u8 field_C; u8 padD[3]; u8 field_10; } Obj;
extern u8 D_8015FE80[];
Obj *func_80115690(Obj *obj, s32 kind);
Obj *func_80124400(Obj *obj) {
    Obj *self = obj;

    func_80115690(obj, 0xD5);
    self->vtable = D_8015FE80;
    self->field_10 = 0;
    self->field_C |= 8;
    return self;
}
