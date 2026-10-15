#include "common.h"
#include "item_records.h"

u32 func_800D07D8(ItemPool **owner, s32 index)
{
    u32 count = (*owner)->capacity;
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
