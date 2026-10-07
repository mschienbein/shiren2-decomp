#include "common.h"

typedef struct {
    unsigned char pad0[0x44];
    s32 id;
    unsigned char pad48[8];
    short type;
    unsigned char pad52[0xA];
    short active;
    unsigned char pad5E[2];
} Task;

typedef void (*TaskFunc)(Task *task);

extern s32 D_80139B18;
extern TaskFunc D_80139B1C[];
extern Task D_801D40DC[32];

void func_80055A68(void)
{
    Task *task;
    Task *end;

    if (D_80139B18 != 0) {
        task = D_801D40DC;
        end = task + 32;
        for (; task < end; task++) {
            if (task->id != -1 && task->active == 1) {
                D_80139B1C[task->type](task);
            }
        }
    }
}
