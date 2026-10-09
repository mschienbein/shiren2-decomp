#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;

typedef struct {
    u8 pad0[0xC];
    s16 x;
    u8 padE[0x2];
    s16 z;
    u8 pad12[0x2A];
    u16 anim;
    u8 frame;
    u8 pad3F[0x7];
    u8 alpha;
} Sprite8008A474;
typedef struct {
    u8 pad0[0x4];
    s16 state;
    u8 pad6[0x2];
    u16 phase;
    u8 padA[0xA];
    s32 handle;
    u8 pad18[0x4];
    s32 timer;
    u8 pad20[0x4];
    s32 field_24;
    s32 field_28;
    s32 field_2C;
    u8 pad30[0xC];
    f32 step_x;
    f32 x;
    u8 pad44[0x4];
    f32 step_z;
    f32 z;
    u8 pad50[0xC];
    s32 from_x;
    s32 to_x;
    u8 pad64[0x4];
    s32 from_z;
    s32 to_z;
} Obj8008A474;
s32 func_800748F8(s32 handle, s32 a, s32 x, s32 z, s32 b, s32 c);
Sprite8008A474 *func_8007946C(s32 kind, s32 handle);
s32 func_80076044(s32 handle, s32 anim, s32 mode, s32 frame, s32 flags);
s32 func_800751B4(s32 handle, s32 flags);
void func_8008A474(Obj8008A474 *obj) {
    Sprite8008A474 *sprite;
    switch (obj->phase) {
    case 0:
        func_800748F8(obj->handle, obj->field_24, obj->from_x, obj->from_z, obj->field_2C, obj->field_28);
        sprite = func_8007946C(0, obj->handle);
        func_80076044(obj->handle, sprite->anim, 2, sprite->frame, 3);
        func_800751B4(obj->handle, 0x8000);
        sprite->alpha = 0;
        obj->timer = 15;
        obj->phase++;
        obj->step_x = (f32)((obj->to_x - obj->from_x) << 7) * 0.0625f;
        obj->step_z = (f32)((obj->to_z - obj->from_z) << 7) * 0.0625f;
        break;
    case 1:
        sprite = func_8007946C(0, obj->handle);
        if (obj->timer-- != 0) {
            s32 base_x = (obj->from_x << 7) + 0x40;
            s32 base_z = (obj->from_z << 7) + 0x40;
            f32 x;
            f32 z;
            obj->x += obj->step_x;
            obj->z += obj->step_z;
            x = obj->x;
            z = obj->z;
            sprite->alpha += 0x10;
            sprite->z = base_z + z;
            sprite->x = base_x + x;
        } else {
            sprite->alpha = 0xFF;
            sprite->x = (obj->to_x << 7) + 0x40;
            sprite->z = (obj->to_z << 7) + 0x40;
            func_80076044(obj->handle, sprite->anim, 1, sprite->frame, 3);
            obj->state = 4;
        }
        break;
    }
}
