#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

typedef struct {
    u8 pad0[2];
    u8 flags2;
} Obj800AF72C;

void func_800AF72C(Obj800AF72C *obj) {
    obj->flags2 &= ~1;
}
