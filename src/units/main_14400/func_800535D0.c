#include "common.h"

typedef unsigned char u8;
typedef struct Sound {
    short index;
    short pad_02;
    s32 id_04;
    u8 volume_08;
} Sound;
typedef struct ObservedFields_80053590 {
    Sound *sound;
    short delta_04;
    u8 duration_06, target_07, initial_08, elapsed_09;
} ObservedFields_80053590;
extern u8 func_80052694(short index);
extern s32 func_8012A534(s32 id, s32 value);
extern s32 func_8012A434(s32 id, s32 value);
extern void func_80053590(ObservedFields_80053590 *record);

void func_800535D0(s32 mode, ObservedFields_80053590 *record, s32 volume, u8 duration)
{
    s32 target = volume;
    s32 initial = record->sound->volume_08;
    if (mode == 2)
        target = (u8)target;
    else
        target = ((u8)func_80052694(record->sound->index) * (u8)target) >> 7;
    if (!duration) {
        if (target)
            func_8012A534(record->sound->id_04, target);
        else
            func_8012A434(record->sound->id_04, 0);
        record->sound->volume_08 = target;
        func_80053590(record);
    } else {
        s32 increasing, difference;
        record->target_07 = target;
        record->duration_06 = duration;
        if (initial < target) {
            increasing = 1;
            difference = target - initial;
        } else {
            increasing = 0;
            difference = initial - target;
        }
        if (!difference) {
            func_80053590(record);
        } else {
            record->target_07 = target;
            record->initial_08 = initial;
            record->duration_06 = duration;
            record->elapsed_09 = 0;
            record->delta_04 = difference;
            if (!increasing)
                record->delta_04 = -difference;
        }
    }
}
