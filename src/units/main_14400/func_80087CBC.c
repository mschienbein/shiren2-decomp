#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef float f32;
typedef double f64;

/* 0x74-byte task record (D_801BA380 table); this handler's view. */
typedef struct {
    void (*handler)(void *);
    u16 state_04;
    u16 unk6;
    u16 phase_08;
    u16 unkA;
    u16 unkC;
    u16 unkE;
    u16 unk10;
    u16 flags_12;
    s32 id_14;
    s32 unk18;
    s32 timer_1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    f32 velocity_30;
    unsigned char pad34[0x74 - 0x34];
} Task;

/* Sprite record returned by func_8007946C; only the fields used here. */
typedef struct {
    unsigned char pad0[0x14];
    f32 y_14;
    unsigned char pad18[0x3C - 0x18];
    u16 animation_3C;
    unsigned char pad3E[0x46 - 0x3E];
    u8 alpha_46;
    unsigned char pad47[0x70 - 0x47];
    u8 colors_70[8]; /* two RGBA colours */
} Sprite;

extern s32 func_800751B4(s32 id, s32 flags);
extern s32 func_80076044(s32 id, s32 animation, s32 mode, s32 command, s32 flags);
extern Sprite *func_8007946C(s32 side, s32 index);
extern void func_80079560(s32 side, s32 id, s32 mode);

void func_80087CBC(Task *task)
{
    Sprite *sprite = func_8007946C(0, task->id_14);
    u8 *color = sprite->colors_70;

    switch (task->phase_08) {
    case 0:
        func_80079560(0, task->id_14, 0);
        if (!(task->flags_12 & 0x4000)) {
            func_80076044(task->id_14, sprite->animation_3C, 2, 6, 0);
        }
        func_800751B4(task->id_14, 0x8000);
        sprite->colors_70[0] = 0;
        color[1] = 0;
        color[2] = 0;
        color[3] = 0xFF;
        color[4] = 0;
        color[5] = 0;
        color[6] = 0;
        color[7] = 0xFF;
        sprite->y_14 = -101.0f;
        task->velocity_30 = 21.0f;
        sprite->alpha_46 = 0;
        task->timer_1C = 15;
        task->phase_08++;
        return;
    case 1:
        if (task->timer_1C-- != 0) {
            color[3] -= 10;
            sprite->y_14 += task->velocity_30;
            task->velocity_30 = task->velocity_30 * 0.8;
            sprite->alpha_46 += 15;
            return;
        }
        sprite->alpha_46 = 0xFF;
        func_80076044(task->id_14, 0, 1, 8, 3);
        func_800751B4(task->id_14, 0);
        task->state_04 = 4;
        return;
    }
}
