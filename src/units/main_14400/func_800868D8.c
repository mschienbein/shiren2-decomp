#include "common.h"
typedef signed short s16;
typedef struct { unsigned char pad0[4]; s16 field4; unsigned char pad6[0xE]; u32 field14; unsigned char pad18[0xC]; s32 field24; } Task;
extern s32 func_8005B058(void);
extern void func_8005A670(s32 mode, s32 save, s32 instant);
void func_800868D8(void *arg) {
    Task *task = arg;
    if ((u32)func_8005B058() < task->field14 || task->field24) {
        func_8005A670(task->field14, task->field24, 0);
    }
    task->field4 = 4;
}
