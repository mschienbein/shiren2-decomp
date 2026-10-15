#include "func_801E8758_13A678/types.h"
extern s32 func_8005652C(s32, s32, s32, s32, s32, s32, s32, s32, s32);
void func_801E8758_13A678(s32 duration, s32 start, s32 end) {
    S06Ramp *ramp;
    S06Task *task;
    D_8013DF70 = start;
    ramp = &D_801F6F0B_148E2B;
    ramp->field1 = start;
    ramp->field2 = end;
    ramp->field3 = duration;
    ramp->field0 = 0;
    ramp = &D_801F6F0F_148E2F;
    ramp->field1 = start;
    ramp->field2 = end;
    ramp->field3 = duration;
    ramp->field0 = 0;
    D_801F5140_147060 = func_8005652C(0x187, 6, 160, 216, 0, -1, -1, -1, -1);
    task = &D_801D40DC[D_801F5140_147060];
    task->color14.bytes[0] = 255;
    task->color14.bytes[1] = 255;
    task->color14.bytes[2] = 255;
    task->color14.bytes[3] = start;
    task->color18.bytes[0] = 0;
    task->color18.bytes[1] = 0;
    task->color18.bytes[2] = 0x41;
    task->color18.bytes[3] = 255;
    task->scale34 = 1.0f;
    task->scale38 = 1.0f;
}
