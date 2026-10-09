#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef float f32;

typedef struct Vec3f800861C0 {
    f32 x;
    f32 y;
    f32 z;
} Vec3f800861C0;

/* Task record (0x74-byte D_801BA380 element) driving a moving effect; the mover fields
 * from 0x1C on are the ones func_800892B0 prepares. */
typedef struct Task800861C0 {
    void (*handler_00)(void *);
    u16 field04;
    u16 field06;
    u16 state08;
    u16 counter0A;
    u8 pad0C[0x12 - 0xC];
    u16 flags12;
    u8 pad14[0x1C - 0x14];
    s32 steps_1C;
    u8 pad20[0x2A - 0x20];
    s16 sound_2A;
    s32 model_2C;
    f32 frame_30;
    f32 speed_34;
    u8 pad38[4];
    f32 stepX_3C;
    f32 posX_40;
    u8 pad44[4];
    f32 stepZ_48;
    f32 posZ_4C;
    u8 pad50[0x5C - 0x50];
    s32 targetX_5C;
    s32 currentX_60;
    s32 arg_64;
    s32 targetZ_68;
    s32 currentZ_6C;
    s32 arg_70;
} Task800861C0;

extern s32 D_801C33B8[4]; /* live effect handles, -1 = free */
extern s32 D_801C33C8;    /* spawn permitted */

void func_800892B0(Task800861C0 *self, f32 speed);
s32 func_8008C1C8(s32 index, float delta);
float func_8008C440(s32 index);
void func_80052260(short arg0);
void func_8008C194(s32 arg0);
s32 func_80084014(s32 x, s32 z);
u32 func_8002A9B0(void);
s32 func_8008BF14(s32, s32, s32, s32, s32, void *, f32);
void func_8008C334(s32);

/* Task handler: spawn up to four trail effects along the mover's path. */
void func_800861C0(Task800861C0 *task)
{
    Vec3f800861C0 pos;
    s32 i;
    s32 result;
    s32 free_count;
    s32 mode;
    s32 handle;
    s32 cell_x;
    s32 cell_z;
    s32 blocked = 0; /* never set in this routine; the original keeps it in s6 */

    switch (task->state08) {
    case 0:
        for (i = 0; i < 4; i++) {
            D_801C33B8[i] = -1;
        }
        func_800892B0(task, task->speed_34);
        task->stepX_3C *= 0.25f;
        task->stepZ_48 *= 0.25f;
        D_801C33C8 = 1;
        task->posX_40 -= task->stepX_3C;
        task->posZ_4C -= task->stepZ_48;
        task->counter0A = 0;
        task->state08++;
        /* fall through */
    case 1:
        for (i = 0; i < 4; i++) {
            if (D_801C33B8[i] < 0) {
                continue;
            }
            result = func_8008C1C8(D_801C33B8[i], 1.0f);
            if (task->counter0A == 0 && func_8008C440(D_801C33B8[i]) == task->frame_30) {
                func_80052260(task->sound_2A);
                task->counter0A++;
            }
            if (result & 2) {
                D_801C33C8 = 1;
            }
            if (result & 1) {
                func_8008C194(D_801C33B8[i]);
                D_801C33B8[i] = -1;
            }
        }
        free_count = 0;
        for (i = 0; i < 4; i++) {
            if (D_801C33C8 != 0 && task->steps_1C != 0 && D_801C33B8[i] == -1) {
                D_801C33C8 = 0;
                mode = !((task->flags12 >> 5) & 1);
                do {
                    task->steps_1C--;
                    task->posX_40 -= task->stepX_3C;
                    task->posZ_4C -= task->stepZ_48;
                    pos.x = task->targetX_5C * 32 + 16;
                    pos.y = 0.0f;
                    pos.z = task->targetZ_68 * 32 + 16;
                    pos.x += (s32)task->posX_40;
                    cell_x = pos.x * 0.03125f;
                    pos.z += (s32)task->posZ_4C;
                    cell_z = pos.z * 0.03125f;
                } while ((task->flags12 & 0x100) && !blocked && task->steps_1C != 0
                         && !func_80084014(cell_x, cell_z));
                if (task->flags12 & 0x400) {
                    s32 jitter = func_8002A9B0();

                    pos.x += (jitter & 0xF) - 7;
                    pos.z += ((jitter >> 3) & 0xF) - 7;
                }
                handle = func_8008BF14(task->model_2C, task->arg_64, task->arg_70, mode, 0, &pos, 1.0f);
                func_8008C334(handle);
                D_801C33B8[i] = handle;
            }
            if (D_801C33B8[i] == -1) {
                free_count++;
            }
        }
        if (free_count == 4 && task->steps_1C == 0) {
            task->field04 = free_count;
        }
        break;
    }
}
