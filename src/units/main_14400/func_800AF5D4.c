#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { u8 field_0; u8 field_1; } Obj;
s32 func_800AD468(u32 bit);
void func_800AF5D4(Obj *obj) {
    func_800AD468(obj->field_1);
}
