#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct { u8 pad0[0xBC]; s32 field_BC; } Obj800FA1C4;
s32 func_800E0F40(Obj800FA1C4 *obj);

void func_800FA1C4(Obj800FA1C4 *obj) {
    if ((u8)func_800E0F40(obj) == 3) {
        obj->field_BC = 1;
    }
}
