#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x1F];
    u8 field_1F;
} Obj800A7DCC;

u8 func_800A7DCC(Obj800A7DCC *obj)
{
    return obj->field_1F;
}
