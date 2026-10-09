#include "common.h"
typedef short s16;
typedef struct { char pad_0[4]; s16 field_4; char pad_6[8]; s16 field_E; char pad_10[6]; s16 field_16; } Task;
extern void func_800522FC(short id);
void func_8008AF94(void *self) {
    Task *task = self;
    func_800522FC(task->field_16);
    task->field_E = 1;
    task->field_4 = 4;
}
