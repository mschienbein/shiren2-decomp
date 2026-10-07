#include "common.h"

typedef unsigned char u8;
typedef signed short s16;

typedef struct Task8005652C Task8005652C;

struct Task8005652C {
    u8 pad0[0x48];
    s32 kind;
    u8 pad4C[0x4];
    s16 state;
    s16 field_52;
    s16 field_54;
    s16 field_56;
    s16 field_58;
    s16 field_5A;
    u8 pad5C[0x4];
};

typedef void (*TaskFunc8005652C)(Task8005652C *task);

extern s32 D_8013A260;
extern TaskFunc8005652C D_8013A264[];
extern Task8005652C D_801D40DC[];

s32 func_800554C4(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

s32 func_8005652C(s32 arg0, s32 kind, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    s32 index;
    Task8005652C *task;

    if (D_8013A260 == 0) {
        return -1;
    }
    index = func_800554C4(arg0, arg2, arg3, arg4);
    if (index != -1) {
        task = &D_801D40DC[index];
        task->state = 0;
        task->kind = (u8)kind;
        task->field_52 = arg5;
        task->field_54 = arg6;
        task->field_56 = arg7;
        task->field_58 = arg8;
        task->field_5A = 0;
        D_8013A264[task->state](task);
    }
    return index;
}
