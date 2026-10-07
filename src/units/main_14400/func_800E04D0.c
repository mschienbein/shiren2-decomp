#include "common.h"

typedef unsigned char u8;
typedef signed char s8;

typedef struct {
    u8 pad0[0x33];
    u8 field_33;
    u8 pad34[0xF];
    u8 field_43;
} Obj;

extern u32 func_800E110C(Obj *obj);

s32 func_800E04D0(Obj *obj)
{
    if (obj->field_43 != 0) {
        return (s8)func_800E110C(obj);
    }
    return obj->field_33;
}
