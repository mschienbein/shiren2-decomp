#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad0[0x3E]; u8 field_3E; } Target8008767C;
typedef struct {
    u8 pad0[4];
    s16 field_4;
    u8 pad6[2];
    u16 state;
    u16 counter;
    u8 padC[8];
    s32 field_14;
    u8 pad18[4];
    s32 timer;
    u8 pad20[4];
    s32 field_24;
} Obj8008767C;
Target8008767C *func_8007946C(s32 arg0, s32 arg1);
s32 func_80076044(s32 slot, s32 animation, s32 mode, s32 frame, s32 flags);
void func_8008767C(Obj8008767C *obj) {
    Target8008767C *target = func_8007946C(0, obj->field_14);

    switch (obj->state) {
    case 0:
        func_80076044(obj->field_14, 0x91, 2, obj->field_24 * 3 + 9, 0);
        obj->timer = 2;
        obj->state++;
        break;
    case 1:
        if (obj->timer-- == 0) {
            func_80076044(obj->field_14, 0x163, 2, obj->field_24 * 2 + 8, 0);
            obj->timer = 2;
            obj->counter = 0;
            obj->state++;
        }
        break;
    case 2:
        if (obj->timer-- == 0) {
            obj->timer = 2;
            obj->counter++;
            if (obj->counter & 1) {
                target->field_3E++;
            } else {
                target->field_3E--;
            }
            if (obj->counter >= 3) {
                obj->timer = 10;
                func_80076044(obj->field_14, 0x91, 2, obj->field_24 * 3 + 9, 0);
                obj->state++;
            }
        }
        break;
    case 3:
        if (obj->timer-- == 0) {
            func_80076044(obj->field_14, 0x91, 1, 8, 3);
            obj->field_4 = 4;
        }
        break;
    }
}
