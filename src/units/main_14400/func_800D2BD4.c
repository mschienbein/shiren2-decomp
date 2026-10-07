#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct {
    s8 field_0;
} Obj;

s32 func_800D2BD4(Obj *obj) {
    return obj->field_0;
}
