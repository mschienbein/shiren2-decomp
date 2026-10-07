#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x9A];
    u16 flags9A;
} Obj800F3B10;

void func_800F3B10(Obj800F3B10 *obj) {
    obj->flags9A &= ~0x10;
}
