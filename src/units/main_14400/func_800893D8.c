#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef float f32;

typedef struct {
    u8 pad0[0xC];
    s16 x_C;
    s16 z_E;
    s16 y_10;
    u8 pad12[0x2];
    f32 scale_14;
} Sprite_800893D8;

typedef struct {
    u8 pad0[0x4];
    s16 mode_4;
    u8 pad6[0x2];
    u16 state_8;
    u8 padA[0x8];
    u16 flags_12;
    s32 handle_14;
    u8 pad18[0x4];
    u32 count_1C;
    u8 pad20[0x10];
    f32 scale_30;
    u8 pad34[0x8];
    f32 step_x_3C;
    f32 offset_x_40;
    u8 pad44[0x4];
    f32 step_y_48;
    f32 offset_y_4C;
    u8 pad50[0xC];
    s32 tile_x_5C;
    s32 target_x_60;
    u8 pad64[0x4];
    s32 tile_y_68;
    s32 target_y_6C;
} Obj_800893D8;

static inline void sprite_place(Sprite_800893D8 *sprite, s32 tile_x, s32 tile_y) {
    sprite->x_C = (tile_x << 7) + 0x40;
    sprite->y_10 = (tile_y << 7) + 0x40;
}

extern Sprite_800893D8 *func_8007946C(s32 kind, s32 handle);
extern s32 func_800627C4(void);
extern void func_800892B0(Obj_800893D8 *obj, f32 speed);
extern void func_80079560(s32 kind, s32 handle, s32 flags);
extern void func_8007935C(s32 handle);
extern s32 func_80084014(s32 x, s32 y);

void func_800893D8(Obj_800893D8 *obj) {
    Sprite_800893D8 *sprite = func_8007946C(4, obj->handle_14);
    s32 x;
    s32 y;

    switch (obj->state_8) {
        case 0:
            if (obj->tile_x_5C == obj->target_x_60 && obj->tile_y_68 == obj->target_y_6C) {
                obj->count_1C = 0;
            } else {
                sprite_place(sprite, obj->tile_x_5C, obj->tile_y_68);
                if (obj->flags_12 & 0x2000) {
                    sprite->z_E = -0x80;
                } else if (func_800627C4() != 2) {
                    sprite->z_E = -0x20;
                } else {
                    sprite->z_E = -0x10;
                }
                if (obj->flags_12 & 0x1000) {
                    sprite->scale_14 = 7.0f;
                    func_800892B0(obj, 2.0f);
                } else {
                    sprite->scale_14 = 15.0f;
                    func_800892B0(obj, 1.0f);
                }
                func_80079560(4, obj->handle_14, 0);
            }
            obj->scale_30 = sprite->scale_14;
            obj->state_8++;
            break;
        case 1:
            if (obj->count_1C == 0) {
                func_8007935C(obj->handle_14);
                obj->mode_4 = 4;
                break;
            }
            do {
                obj->offset_x_40 -= obj->step_x_3C;
                obj->offset_y_4C -= obj->step_y_48;
                sprite_place(sprite, obj->tile_x_5C, obj->tile_y_68);
                sprite->x_C += (s32)obj->offset_x_40;
                sprite->y_10 += (s32)obj->offset_y_4C;
                if (obj->flags_12 & 0x1000) {
                    sprite->scale_14 = obj->scale_30 + (f32)((obj->count_1C & 1) * 3);
                }
                x = (sprite->x_C - 0x40) >> 7;
                y = (sprite->y_10 - 0x40) >> 7;
                obj->count_1C--;
                if (!(obj->flags_12 & 0x100) || obj->count_1C == 0) {
                    break;
                }
            } while (func_80084014(x, y) == 0);
            break;
    }
}
