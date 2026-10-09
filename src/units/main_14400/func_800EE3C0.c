#include "common.h"
/* 0x34-byte embedded subobject constructed by overlay func_801F2AC0, which initialises
 * fields at +0x00..+0x2D (opaque here) and its vtable word at +0x30. */
typedef struct { unsigned char opaque[0x30]; void *vtable; } Sub801F2AC0;
typedef struct { unsigned char pad0[0x1E]; unsigned char field_1E; unsigned char pad1F[5]; void *vtable_24; unsigned char pad28[0x5C]; Sub801F2AC0 sub_84; } Obj800EE3C0;
extern unsigned char D_80159130[];
extern unsigned char D_80159150[];
/* Constructor: explicitly returns its receiver (0x800E8764, 0x800E8770); result discarded here. */
Obj800EE3C0 *func_800E8730(Obj800EE3C0 *obj);
void func_801F2AC0(Sub801F2AC0 *sub);
s32 func_800A3934(Obj800EE3C0 *obj);
void func_800EE448(Obj800EE3C0 *obj, s32 a, unsigned char b);
Obj800EE3C0 *func_800EE3C0(Obj800EE3C0 *obj, s32 a, unsigned char b) {
    func_800E8730(obj);
    func_801F2AC0(&obj->sub_84);
    obj->sub_84.vtable = D_80159130;
    obj->vtable_24 = D_80159150;
    if (func_800A3934(obj) != 0) return obj;
    obj->field_1E = 8;
    func_800EE448(obj, (unsigned char)a, b);
    return obj;
}
