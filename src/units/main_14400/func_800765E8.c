#include "common.h"

typedef signed char s8;
typedef short s16;

typedef struct {
    s8 count;
    s8 pad1;
    s16 pos[3][3];
    s16 rot[3][3];
    s16 time[3];
    s16 lastTime;
} Entry;

extern Entry D_801A79E8[];

s32 func_800765E8(s32 index, s32 x, s32 y, s32 z, s32 rx, s32 ry, s32 rz, s32 time)
{
    Entry *entry = &D_801A79E8[index];
    s32 slot;

    if (entry->count >= 0) {
        if (entry->count >= 3) {
            slot = -1;
            entry->count = 3;
        } else {
            slot = entry->count++;
        }
    } else {
        slot = -1;
    }

    if (slot == -1) {
        entry->rot[2][0] = rx;
        entry->rot[2][1] = ry;
        entry->rot[2][2] = rz;
        entry->time[2] = time;
    } else {
        entry->pos[slot][0] = x;
        entry->pos[slot][1] = y;
        entry->pos[slot][2] = z;
        entry->rot[slot][0] = rx;
        entry->rot[slot][1] = ry;
        entry->rot[slot][2] = rz;
        entry->time[slot] = time;
    }
    entry->lastTime = time;
    return 1;
}
