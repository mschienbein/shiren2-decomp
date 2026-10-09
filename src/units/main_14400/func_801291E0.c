#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct {
    u8 pad0[0xAE];
    u16 field_AE;
} Obj801291E0;

u8 *func_801291E0(Obj801291E0 *obj, u8 *p)
{
    u32 value;

    value = *p++;
    if (value & 0x80) {
        value = *p++ | ((value & 0x7F) << 8);
    }
    obj->field_AE = value;
    return p;
}
