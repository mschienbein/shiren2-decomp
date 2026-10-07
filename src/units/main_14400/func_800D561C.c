#include "common.h"

typedef unsigned char u8;

void *func_800AFD78(void *table, u8 index);
s32 func_800AC670(void *ptr);

void *func_800D561C(void **arg0, u8 arg1) {
    void *ptr = func_800AFD78(*arg0, arg1);
    if (ptr == 0 || func_800AC670(ptr) != 0) {
        return 0;
    }
    return ptr;
}
