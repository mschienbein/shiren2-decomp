#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 pad0[4];
    u16 active;
    u8 pad6[0xE];
    s32 field_14;
    s32 field_18;
    s32 field_1C;
    u8 pad20[0xC];
    s32 field_2C;
    u8 pad30[0x44];
} Task;

extern Task D_801BA380[];

/* func_80084CD4 allocates a task slot, stores the handler at +0x0, returns the slot index. */
s32 func_80084CD4(void (*handler)(Task *task));
void func_80085848(Task *task);

void func_80085200(s32 arg0, s32 arg1, u16 arg2, s32 arg3) {
    Task *task = &D_801BA380[func_80084CD4(func_80085848)];

    task->field_1C = arg2;
    task->field_14 = arg0;
    task->field_18 = arg1;
    task->field_2C = arg3;
    if (task->active == 0) {
        task->active = 1;
    }
}
