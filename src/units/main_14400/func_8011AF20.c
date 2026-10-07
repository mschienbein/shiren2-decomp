#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { u8 pad0[8]; void *vtable; } Obj;
extern u8 D_8015E750[];
Obj *func_80112D20(Obj *obj, s32 kind);
void func_800ACF34(Obj *obj);
Obj *func_8011AF20(Obj *obj) {
    func_80112D20(obj, 0x20);
    obj->vtable = D_8015E750;
    func_800ACF34(obj);
    return obj;
}
