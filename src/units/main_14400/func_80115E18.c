#include "common.h"
typedef struct { unsigned char field_00[12]; unsigned char field_0C; } Object;
typedef struct { s32 x, y; } Pos;
extern s32 func_80049CB4(s32 value, ...);
void func_80115E18(Object *obj, Pos *position)
{
    obj->field_0C |= 0x24;
    func_80049CB4(0xE3, position);
}
