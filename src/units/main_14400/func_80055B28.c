#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef struct Task Task;
struct Task {
    u8 pad0[0x48];
    s32 field_48;
    u8 pad4C[4];
    s16 state;
    s16 field_52;
    u8 pad54[4];
    s16 field_58;
    s16 field_5A;
    u8 pad5C[4];
};
extern s32 D_80139B18;
extern void (*D_80139B1C[])(Task *task);
extern Task D_801D40DC[];
s32 func_800554C4(s32 a, s32 b, s32 c, s32 d);
s32 func_80055B28(s32 a, s32 kind, s32 b, s32 c, s32 d) {
    s32 index;
    Task *task;

    if (D_80139B18 == 0) {
        return -1;
    }
    index = func_800554C4(a, b, c, d);
    if (index != -1) {
        task = &D_801D40DC[index];
        task->state = 0;
        task->field_48 = (u8)kind;
        task->field_52 = 0x10;
        task->field_58 = 0x10;
        task->field_5A = 0;
        D_80139B1C[task->state](task);
    }
    return index;
}
