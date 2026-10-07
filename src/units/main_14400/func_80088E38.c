#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef short s16;

typedef struct {
    u8 pad0[0x4];
    s16 state;
    u8 pad6[0xE];
    s32 field_14;
} Obj;

extern s8 *func_80079448(s32 arg);

void func_80088E38(Obj *obj)
{
    if (*func_80079448(obj->field_14) == 0) {
        obj->state = 4;
    }
}
