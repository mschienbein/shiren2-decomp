#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct {
    u8 pad00[0x2A];
    u16 value2A;
    u8 pad2C[6];
    u8 value32;
    u8 pad33[8];
    u8 flag3B;
    u8 pad3C;
    u8 flag3D;
} Obj800E0F40;
extern s32 func_800E0F40(Obj800E0F40 *obj);

u16 func_800E08F0(void *ptr)
{
    Obj800E0F40 *obj = ptr;
    s32 value = obj->value2A;
    if (obj->flag3D) {
        u8 level = obj->value32;
        value -= (level - (u8)func_800E0F40(obj)) * 5;
        if (value <= 0) value = 1;
    }
    if (obj->flag3B) {
        value = (value + 1) / 2;
    }
    return value;
}
