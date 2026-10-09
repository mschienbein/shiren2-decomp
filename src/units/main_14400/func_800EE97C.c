#include "common.h"

typedef unsigned char u8;
typedef struct Target800E95B8 Target800E95B8;
/* 0x34-byte overlay secondary base at +0x84, constructed by func_801F2AC0 (fields
 * +0x00..+0x2F opaque here, vtable word at +0x30 = D_80159130). */
typedef struct { unsigned char opaque[0x30]; void *vtable; } Sub801F2AC0;
typedef struct { unsigned char pad00[0x84]; Sub801F2AC0 sub_84; } Obj;
extern void func_800E95B8(u8 *obj, Target800E95B8 *target);
extern void func_801F2C1C(Sub801F2AC0 *sub, Target800E95B8 *target);

void func_800EE97C(Obj *obj, Target800E95B8 *target)
{
    func_800E95B8((u8 *)obj, target);
    func_801F2C1C(obj != 0 ? &obj->sub_84 : 0, target);
}
