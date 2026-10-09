#include "common.h"

typedef unsigned char u8;

typedef struct Obj Obj;

/* 0x34-byte overlay secondary base at +0x84, constructed by func_801F2AC0 (fields
 * +0x00..+0x2F opaque here, vtable word at +0x30 = D_80159130). */
typedef struct { unsigned char opaque[0x30]; void *vtable; } Sub801F2AC0;

typedef struct {
    u8 pad0[0x84];
    Sub801F2AC0 sub_84;
} Self800EE9C0;

void func_800E9614(Self800EE9C0 *self, Obj *obj);
void func_801F2CB8(Sub801F2AC0 *base, Obj *obj);

void func_800EE9C0(Self800EE9C0 *self, Obj *obj)
{
    func_800E9614(self, obj);
    func_801F2CB8(self != 0 ? &self->sub_84 : 0, obj);
}
