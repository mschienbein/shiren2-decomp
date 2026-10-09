#include "common.h"
typedef unsigned char u8;
typedef struct Obj800E02D0 Obj800E02D0;
/* 0x34-byte overlay secondary base at +0x78, constructed by func_801F2AC0 in
 * func_800EF930 (vtable word at +0x30 = obj+0xA8, D_80159300). */
typedef struct { unsigned char opaque[0x30]; void *vtable; } Sub801F2AC0;
typedef struct { u8 pad_0[0x78]; Sub801F2AC0 sub_78; } Obj;
extern void func_800E02D0(u8 *arg0, Obj800E02D0 *obj);
extern void func_801F2C1C(Sub801F2AC0 *sub, Obj800E02D0 *stream);
void func_800EFB74(Obj *obj, Obj800E02D0 *stream) {
    func_800E02D0((u8 *)obj, stream);
    func_801F2C1C(obj ? &obj->sub_78 : 0, stream);
}
