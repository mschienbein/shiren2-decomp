#include "common.h"

extern void *func_800CDAF4(void *collection, void *source);
extern s32 func_800CD508(void *container, const void *requirement);

s32 func_800CD4C4(void *container, void *item) {
    if (func_800CDAF4(container, item) != 0) {
        return 1;
    }
    return func_800CD508(container, item);
}
