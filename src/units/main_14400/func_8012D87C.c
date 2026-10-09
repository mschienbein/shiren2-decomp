#include "common.h"
typedef unsigned char u8;

typedef struct {
    u8 *base;
    u8 *cur;
    s32 len;
    s32 count;
} ALHeap;
extern ALHeap D_801CA940;

s32 func_8012D87C(void) {
    return D_801CA940.cur - D_801CA940.base;
}
