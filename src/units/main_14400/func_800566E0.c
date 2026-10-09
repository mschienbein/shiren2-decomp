#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
/* 0x60-byte fade task; only the fields used here are named. */
typedef struct {
    u8 pad_00[0x17];
    u8 alpha_17;
    u8 pad_18[0x1C];
    float scale_x_34, scale_y_38;
    u8 pad_3C[0xC];
    u32 kind_48;
    u8 pad_4C[4];
    short state_50, duration_52;
    u8 pad_54[6];
    short elapsed_5A;
    u8 pad_5C[4];
} Task;

#define HALF_PI 1.5707963267948966

float func_80032360(float x); /* sinf */

/* Advances a fade-in: alpha follows 1 - sin of the remaining fraction, and
 * kinds 3..5 and 7+ also pulse their scale. */
void func_800566E0(Task *task) {
    u16 elapsed = task->duration_52;

    if ((short)elapsed != -1) {
        s32 remaining;
        u8 *alpha;
        float phase;

        if ((short)elapsed >= task->elapsed_5A + 1) elapsed = task->elapsed_5A + 1;
        remaining = 255 - (s32)(((float)(short)elapsed / (float)task->duration_52) * 255.0f);
        task->elapsed_5A = elapsed;
        alpha = &task->alpha_17;
        *alpha = (u32)((1.0f - func_80032360((float)(((double)(float)remaining * HALF_PI) / 255.0))) * 255.0f);
        if (task->kind_48 >= 3U && task->kind_48 != 6) {
            phase = ((double)(float)(255 - remaining) * HALF_PI) / 255.0;
            task->scale_x_34 = 1.1f - func_80032360(phase) * 0.1f;
            task->scale_y_38 = 1.1f - func_80032360(phase) * 0.1f;
        } else {
            task->scale_x_34 = 1.0f;
            task->scale_y_38 = 1.0f;
        }
        if (*alpha >= 255U) {
            task->state_50 = 2;
            task->elapsed_5A = 0;
            task->scale_x_34 = 1.0f;
            task->scale_y_38 = 1.0f;
        }
    }
}
