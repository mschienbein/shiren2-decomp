#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct { u8 pad0[0x44]; s32 field_44; } Obj80043F9C;
extern s32 D_80138B00;
void func_80043F58(Obj80043F9C *obj);

void func_80043F9C(Obj80043F9C *obj) {
    if (D_80138B00 == 0 && obj->field_44 != 0) {
        func_80043F58(obj);
    }
}
