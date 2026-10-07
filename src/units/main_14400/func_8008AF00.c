#include "common.h"

typedef short s16;
typedef unsigned short u16;

typedef struct {
    unsigned char pad0[4];
    s16 field_4;
    unsigned char pad6[2];
    u16 state;
    unsigned char padA[4];
    s16 field_E;
    unsigned char pad10[6];
    s16 field_16;
    unsigned char pad18[4];
    s32 timer;
    unsigned char pad20[4];
    s32 duration;
} Obj;

void func_80052260(s16 arg0);

void func_8008AF00(Obj *obj) {
    switch (obj->state) {
    case 0:
        if (obj->timer != 0) {
            obj->timer--;
            return;
        }
        func_80052260(obj->field_16);
        obj->timer = obj->duration;
        obj->state++;
    case 1:
        if (obj->timer-- == 0) {
            obj->field_E = 1;
            obj->field_4 = 4;
        }
        break;
    }
}
