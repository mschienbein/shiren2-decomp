#include "func_801E8834_13A754/types.h"
void func_801E8834_13A754(void) {
    S06Ramp *first = &D_801F6F0B_148E2B;
    S06Ramp *second = &D_801F6F0F_148E2F;
    s32 amount, frame;
    S06Task *task;
    if (!first->field3 && !second->field3) return;
    if (first->field3) {
        first->field0++;
        amount = first->field2 - first->field1;
        amount = amount * first->field0 / first->field3;
        D_8013DF70 = amount;
        if (first->field0 == first->field3) first->field3 = 0;
    }
    if (second->field3) {
        second->field0++;
        frame = second->field0 - 24;
        if (frame < 0) frame = 0;
        amount = second->field2 - second->field1;
        amount = amount * frame / second->field3;
        task = &D_801D40DC[D_801F5140_147060];
        if (amount < 256) task->color14.bytes[3] = amount;
        else task->color14.bytes[3] = 255;
        if (frame == second->field3) second->field3 = 0;
    }
}
