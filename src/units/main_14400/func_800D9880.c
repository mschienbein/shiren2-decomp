#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct {
    u8 pad0[4];
    void *field_4;
} Obj;

extern s32 D_80158098;
void func_800DDAD0(Obj *, s32);

Obj *func_800D9880(Obj *obj) {
    func_800DDAD0(obj, 9);
    obj->field_4 = &D_80158098;
    return obj;
}
