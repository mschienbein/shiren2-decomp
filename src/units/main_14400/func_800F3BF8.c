#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct {
    u8 pad0[0x9A];
    s16 field_9A;
} Obj;

void func_800F3BF8(Obj *obj) {
    obj->field_9A = 0;
}
