#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef union { s32 word; struct { u16 hi, lo; } half; } Pos;
typedef struct {
    u8 pad_00[2]; short kind_02; u8 pad_04[2]; u8 level_06; u8 pad_07[2]; u8 mode_09; u8 pad_0A[2];
    short x_0C, height_0E, y_10; u8 pad_12[2]; float elevation_14; u8 pad_18[8]; float scale_20;
    u8 pad_24[0x1A]; u8 pose_3E; u8 pad_3F; u8 active_40;
} Sprite;
typedef struct {
    u8 pad_00[4]; short state_04; u8 pad_06[2]; u16 phase_08; u8 pad_0A[8]; u16 flags_12;
    s32 sprite_14; u8 pad_18[4]; s32 timer_1C; u8 pad_20[4]; s32 saved_mode_24, saved_active_28, saved_pose_2C;
    float saved_elevation_30; u8 pad_34[0x14]; float velocity_48; u8 pad_4C[0x10];
    Pos from_x_5C, to_x_60; u8 pad_64[4]; Pos from_y_68, to_y_6C;
} Task;
typedef struct { u8 pad_00[4]; u8 height_04; } Species;
typedef struct { Species *species_00; } SpeciesEntry;
extern Sprite *func_8007946C(s32 side, s32 index);
extern void func_80061820(s32 x, s32 y);
extern void func_80061908(Pos x, Pos y);
extern void func_8005A234(void);
extern s32 func_80042734(s32 id);
extern s32 func_800627C4(void);
extern SpeciesEntry *func_80074784(s32 kind, s32 level);
extern s32 func_800625FC(s32 x, s32 y);
extern s32 func_80062554(s32 x, s32 y);
extern void func_8004194C(s32 id, s32 *out_y, s32 *out_x);
void func_80086E94(Task *task) {
    Sprite *sprite = func_8007946C(0, task->sprite_14);
    s32 x, y, dx, dy;
    switch (task->phase_08) {
    case 0:
        task->saved_mode_24 = sprite->mode_09;
        sprite->mode_09 = 0;
        sprite->x_0C = (task->from_x_5C.half.lo << 7) + 64;
        sprite->y_10 = (task->from_y_68.half.lo << 7) + 64;
        task->saved_active_28 = sprite->active_40;
        task->saved_pose_2C = sprite->pose_3E;
        sprite->active_40 = 0;
        sprite->pose_3E = (task->flags_12 & 0x1000) ? 0x1B : 6;
        task->saved_elevation_30 = sprite->elevation_14;
        task->velocity_48 = 20.0f;
        task->timer_1C = 19;
        task->phase_08++;
        break;
    case 1:
        if (task->timer_1C-- != 0) {
            if (task->flags_12 & 0x2000) {
                if ((u32)task->timer_1C >= 8) {
                    sprite->elevation_14 += task->velocity_48;
                    task->velocity_48 *= 1.85f;
                }
            } else if ((u32)task->timer_1C >= 6) sprite->elevation_14 += task->velocity_48;
            break;
        }
        x = task->to_x_60.word;
        y = task->to_y_6C.word;
        if (task->flags_12 & 2) {
            Pos px, py;
            px.word = x;
            py.word = y;
            func_80061820(x, y);
            func_80061908(px, py);
            func_8005A234();
        }
        sprite->x_0C = (x << 7) + 64;
        sprite->y_10 = (y << 7) + 64;
        switch (func_80042734(task->sprite_14)) {
        case 1:
            if (sprite->kind_02 == 0x49) {
                s32 base = -8;
                if (func_800627C4() == 2) base = -4;
                sprite->height_0E = (base - (s32)(func_80074784(sprite->kind_02, sprite->level_06)->species_00->height_04 * sprite->scale_20)) << 2;
            } else sprite->height_0E = func_80062554(x, y) << 2;
            break;
        case 2:
            if (func_800625FC(x, y) & 0x2000) sprite->height_0E = 0;
            else sprite->height_0E = func_80062554(x, y) << 2;
            break;
        }
        dx = task->from_x_5C.word - task->to_x_60.word;
        dy = task->from_y_68.word - task->to_y_6C.word;
        if (dx == 0 && dy == 0) {
            sprite->elevation_14 = task->saved_elevation_30 + 4.0f;
            task->phase_08++;
        } else {
            sprite->elevation_14 = task->saved_elevation_30 - 2.0f;
            task->phase_08 += 2;
        }
        break;
    case 2:
        sprite->elevation_14 = task->saved_elevation_30 - 2.0f;
        task->phase_08++;
        break;
    case 3:
        sprite->elevation_14 = task->saved_elevation_30;
        sprite->mode_09 = task->saved_mode_24;
        sprite->active_40 = task->saved_active_28;
        sprite->pose_3E = task->saved_pose_2C;
        if (task->flags_12 & 2) {
            s32 out_y, out_x;
            func_8004194C(task->sprite_14, &out_y, &out_x);
        }
        task->state_04 = 4;
        break;
    }
}
