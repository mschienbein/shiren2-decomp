#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct {
    char pad0[7];
    u8 field_7;
    char pad8[0x3E];
    u8 alpha;
} Sprite;

typedef struct {
    char pad0[4];
    s16 field_4;
    char pad6[2];
    u16 state;
    char padA[0xA];
    s32 field_14;
    char pad18[4];
    s32 timer;
} Obj;

extern Sprite *func_8007946C(s32 kind, s32 id);
extern void func_800885B8(Obj *obj);

void func_8008792C(Obj *obj) {
    Sprite *sprite = func_8007946C(0, obj->field_14);

    switch (obj->state) {
    case 0:
        sprite->field_7 = 2;
        sprite->alpha = 0xFF;
        obj->timer = 7;
        obj->state++;
        break;
    case 1:
        if (obj->timer-- != 0) {
            sprite->alpha -= 0x1F;
        } else {
            func_800885B8(obj);
            obj->field_4 = 4;
        }
        break;
    }
}
