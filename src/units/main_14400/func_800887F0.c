#include "common.h"

typedef struct Task Task;

void func_80088828(Task *task);

/* Task callback: forwards the task to the shared update routine. */
void func_800887F0(Task *task)
{
    func_80088828(task);
}
