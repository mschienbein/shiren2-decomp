#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef struct {
    u8 pad_00[7]; u8 mode_07;
    u8 pad_08[4]; s16 x_0C, y_0E, z_10;
    u8 pad_12[0x38]; s16 direction_4A;
} Unit;
typedef struct {
    u8 pad_00[4]; u16 state_04, pad_06, phase_08, frame_0A, pad_0C, signal_0E;
    u8 pad_10[4]; s32 slot_14; u8 pad_18[4]; s32 timer_1C;
    u8 pad_20[0x3C]; s32 saved_x_5C;
    u8 pad_60[8]; s32 saved_z_68;
} Task;
typedef union { s32 word; struct { s16 hi, lo; } half; } Step;
extern s32 D_8013E9A8[8];
/* The eight direction words are followed by four signed bobbing words at
 * 0x8013E9E8; the disassembler's D_8013E9EA is their first low halfword. */
extern struct { s32 direction[8]; Step bob[4]; } D_8013E9C8;
extern Unit *func_8007946C(s32 side, s32 slot);

void func_800874FC(Task *task) {
    Unit *unit = func_8007946C(0, task->slot_14);
    s32 dx, dz;
    unit->mode_07 = 2;
    dx = D_8013E9A8[unit->direction_4A];
    dz = D_8013E9C8.direction[unit->direction_4A];
    switch (task->phase_08) {
    case 0:
        task->timer_1C = 8;
        task->saved_x_5C = unit->x_0C;
        task->saved_z_68 = unit->z_10;
        task->phase_08++;
        break;
    case 1:
        if (task->timer_1C-- != 0) {
            unit->x_0C += dx * 8;
            unit->z_10 += dz * 8;
            unit->y_0E += D_8013E9C8.bob[task->frame_0A++ & 3].half.lo;
            if (task->timer_1C == 5) {
                task->signal_0E = 1;
            }
        } else {
            task->timer_1C = 2;
            task->phase_08++;
        }
        break;
    case 2:
        if (task->timer_1C-- == 0) {
            unit->x_0C = task->saved_x_5C;
            unit->z_10 = task->saved_z_68;
            task->phase_08++;
        }
        break;
    case 3:
        unit->mode_07 = 1;
        task->state_04 = 4;
        break;
    }
}
