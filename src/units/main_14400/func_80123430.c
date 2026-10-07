#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct { u8 pad0[8]; void *field_8; } Obj80123430;
extern u8 D_8015FBC0[];
void func_8010E5C0(Obj80123430 *obj, s32 arg1);

Obj80123430 *func_80123430(Obj80123430 *obj) {
    func_8010E5C0(obj, 0xC9);
    obj->field_8 = D_8015FBC0;
    return obj;
}
