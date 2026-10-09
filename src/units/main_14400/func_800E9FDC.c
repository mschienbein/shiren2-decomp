#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad0[0x7C]; s32 field_7C; } Obj800E9FDC;
extern u32 D_8013960C;
s32 func_800E8928(Obj800E9FDC *obj);
u16 func_800E08F0(Obj800E9FDC *obj);
s32 func_800E0534(void *obj, s32 amount);
void func_800E9FDC(Obj800E9FDC *obj) {
    u16 count;

    D_8013960C <<= 1;
    count = (u8)func_800E8928(obj);
    if (count == 0) {
        obj->field_7C += func_800E08F0(obj);
        while (obj->field_7C >= 200) {
            obj->field_7C -= 200;
            count++;
        }
    }
    if (count != 0) {
        func_800E0534(obj, count);
    }
    D_8013960C >>= 1;
}
