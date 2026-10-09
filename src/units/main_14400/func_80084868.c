#include "common.h"
typedef unsigned short u16;
typedef struct { unsigned char pad_00[4]; u16 state_04; u16 field_06; u16 step_08; unsigned char pad_0A[0xA]; s32 unit_14; } Task80084868;
void func_8007D8A0(s32 mode);
void func_80042888(s32 unit);
void func_80084868(Task80084868 *task)
{
    switch (task->step_08) {
    case 0:
        func_8007D8A0(1);
        task->step_08++;
        break;
    case 1:
        func_80042888(task->unit_14);
        task->state_04 = 4;
        break;
    }
}
