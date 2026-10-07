#include "common.h"

/* Allocation header of the func_800913F4 heap; layout owned by func_80091488. */
typedef struct HeapBlock HeapBlock;

extern void *D_80140070;
void *func_80091488(HeapBlock *b, u32 size);
void *func_80091450(u32 arg) {
    void *result;
    if (D_80140070 != 0) {
        result = func_80091488(D_80140070, arg);
    } else {
        result = 0;
    }
    return result;
}
