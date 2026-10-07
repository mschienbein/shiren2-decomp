#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct {
    u8 pad0[0xD];
    u8 field_D;
} Obj;

void func_8010C874(Obj *obj, s32 value) {
    obj->field_D = value;
}
