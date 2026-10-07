#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    char pad0[0x2C];
    u16 field_2C;
    char pad2E[0xE];
    u8 halve;
} Obj;

u32 func_800E0E88(Obj *obj)
{
    if (obj->halve != 0) {
        return (obj->field_2C + 1U) >> 1;
    }
    return obj->field_2C;
}
