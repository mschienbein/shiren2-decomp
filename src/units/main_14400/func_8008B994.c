#include "common.h"

/* Task handler registered via func_80084C60/func_80085154 (void (*)(void *)); forwards its task. */
void func_8008B9B0(void *task);

void func_8008B994(void *task) {
    func_8008B9B0(task);
}
