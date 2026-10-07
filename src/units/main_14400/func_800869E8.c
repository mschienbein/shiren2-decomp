#include "common.h"

typedef unsigned char u8;
typedef signed short s16;

typedef struct { u8 pad0[4]; s16 x4; u8 pad6[8]; s16 xE; u8 pad10[4]; s32 x14; } Obj;
void func_8005ADAC(s32 a, s32 b, s32 c);
void func_800869E8(Obj *obj) {
    func_8005ADAC(8, 1, obj->x14);
    obj->xE = 1;
    obj->x4 = 4;
}
