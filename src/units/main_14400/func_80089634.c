#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef float f32;

typedef struct Unit8004D170 {
    u8 pad_00[0xC];
    s16 x_0C;
    u8 pad_0E[2];
    s16 y_10;
    u8 pad_12[2];
    f32 z_14;
    u8 pad_18[0x3C - 0x18];
    u16 field_3C;
} Unit8004D170;

typedef struct Task80089634 {
    u8 pad_00[0x4];
    u16 done_04;
    u8 pad_06[2];
    u16 phase_08;
    u8 pad_0A[0x14 - 0xA];
    s32 slot_14;
    u8 pad_18[0x4];
    s32 timer_1C;
    u8 pad_20[0x4];
    s32 target_24;
} Task80089634;

Unit8004D170 *func_8007946C(s32 side, s32 slot);
s32 func_80076044(s32, s32, s32, s32, s32);

/* Task handler: glide unit slot_14 towards unit target_24, then hop and finish. */
void func_80089634(Task80089634 *task) {
    Unit8004D170 *unit = func_8007946C(0, task->slot_14);
    Unit8004D170 *target = func_8007946C(0, task->target_24);

    switch (task->phase_08) {
    case 0:
        func_80076044(task->slot_14, unit->field_3C, 2, 0, 1);
        task->timer_1C = 2;
        task->phase_08++;
        /* fallthrough */
    case 1:
        if (task->timer_1C-- != 0) {
            unit->x_0C += (target->x_0C - unit->x_0C) / 8;
            unit->y_10 += (target->y_10 - unit->y_10) / 8;
        } else {
            task->phase_08++;
        }
        break;
    case 2:
        unit->x_0C = target->x_0C;
        unit->y_10 = target->y_10;
        unit->z_14 += 30.0f;
        task->timer_1C = 2;
        task->phase_08++;
        break;
    case 3:
        if (task->timer_1C-- == 0) {
            task->done_04 = 4;
        }
        break;
    }
}
