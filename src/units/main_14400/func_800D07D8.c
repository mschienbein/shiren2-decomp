#include "common.h"

typedef unsigned char u8;
typedef struct {
    u8 pad[8];
    u32 count;
} List;
s32 func_800AF920(List *, u8);

u32 func_800D07D8(List **owner, s32 index)
{
    u32 count = (*owner)->count;
    s32 seen = 0;
    u32 i;

    i = 0;
    while (i < count) {
        if (func_800AF920(*owner, i)) {
            if (seen++ == index) {
                return i;
            }
        }
        i++;
    }
    i = 0;
    while (i < count) {
        if ((func_800AF920(*owner, i) ^ 1) != 0) {
            if (seen++ == index) {
                return i;
            }
        }
        i++;
    }
    return count;
}
