#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct {
    u8 pad0[0xC4];
    s32 field_C4;
} Obj;

void func_80108548(Obj *obj) {
    obj->field_C4 = 0;
}
