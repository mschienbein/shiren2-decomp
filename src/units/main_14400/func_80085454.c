#include "common.h"

/* Element of the 0x74-byte task table at D_801BA380 (see func_80085200/func_80085318). */
typedef struct Task80085454 {
    unsigned char pad00[4];
    unsigned short field04;
    unsigned short field06; /* 1-based index of the task waited on, 0 = none */
    unsigned short state08;
    unsigned char pad0A[4];
    unsigned short field0E;
    unsigned char pad10[0x64];
} Task80085454;

extern Task80085454 D_801BA380[];

/* Task handler registered through func_80084CD4: wait for another task to reach state 4. */
void func_80085454(Task80085454 *task)
{
    switch (task->state08) {
    case 0:
        if (task->field06 == 0) {
            task->field0E = 1;
            task->field04 = 4;
            break;
        }
        task->state08++;
        /* fall through */
    case 1:
        if (D_801BA380[task->field06 - 1].field04 == 4) {
            task->field0E = 1;
            task->field04 = 4;
        }
        break;
    }
}
