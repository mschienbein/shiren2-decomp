#include "common.h"

typedef struct {
    unsigned char pad0[0x58];
    u32 field_58;
} Obj80097A18;

u32 func_80097A18(Obj80097A18 *obj)
{
    return obj->field_58;
}
