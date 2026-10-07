#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct {
    u8 pad0[0xF];
    s8 field_F;
} Obj;

s32 func_8009A2D8(Obj *);

s32 func_8009A2F8(Obj *obj) {
    s32 ok = func_8009A2D8(obj) == 1;

    if (!ok) {
        return 0;
    }
    return obj->field_F > 0;
}
