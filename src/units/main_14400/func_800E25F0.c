#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef float f32;

typedef struct {
    u8 pad0[0x72];
    u8 flags72;
} Obj800E25F0;

void func_800E25F0(Obj800E25F0 *obj) {
    obj->flags72 |= 8;
}
