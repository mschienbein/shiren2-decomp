#include "common.h"

typedef unsigned char u8;
typedef signed short s16;
typedef struct { u8 pad00[0x50]; s16 field50; u8 pad52[8]; s16 field5A; u8 pad5C[4]; } Task;
extern s32 D_80139B30;
extern Task D_801D40DC[32];
extern void func_80055EDC(s32 index);

void func_80056394(Task *task) {
    if (D_80139B30 == 1) {
        task->field50 = 0;
        task->field5A = 0;
        func_80055EDC(task - D_801D40DC);
    }
}
