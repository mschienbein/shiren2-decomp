#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct {
    u8 pad0[0xB2];
    u16 field_B2;
    u16 field_B4;
} Obj8012945C;

u8 *func_8012945C(Obj8012945C *obj, u8 *p)
{
    u32 value;

    value = *p++;
    obj->field_B2 = 0;
    obj->field_B4 = value;
    return p;
}
