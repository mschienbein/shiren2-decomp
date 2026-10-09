#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 field00;
    u8 pad01[0xF];
    u8 field10;
} Obj800434D0;

extern void func_80062804(u32 mode, s32 arg1, s32 arg2, s32 arg3);

void func_800434D0(Obj800434D0 *obj)
{
    func_80062804(2, obj->field00, obj->field10, 0);
}
