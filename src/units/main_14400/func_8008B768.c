#include "common.h"

typedef unsigned short u16;
typedef float f32;
typedef struct {
    char pad0[4];
    u16 unk4;
    char pad6[2];
    u16 state8;
    char padA[4];
    u16 unkE;
    char pad10[0xC];
    s32 timer1C;
    char pad20[0x10];
    f32 value30;
    f32 start34;
} Obj;
f32 func_80062CAC(void);
void func_80062CBC(f32);
void func_8008B768(Obj *obj) {
    switch (obj->state8) {
    case 0:
        obj->unkE = 1;
        obj->value30 = obj->start34 = func_80062CAC();
        obj->timer1C = 100;
        obj->state8++;
        break;
    case 1:
        obj->value30 -= 0.008f;
        if (obj->timer1C-- == 0) {
            obj->unk4 = 4;
            obj->value30 = obj->start34;
        }
        func_80062CBC(obj->value30);
        break;
    }
}
