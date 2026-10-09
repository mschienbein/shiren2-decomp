#include "common.h"

typedef struct {
    unsigned char pad_00[5];
    signed char index_05;
    unsigned char pad_06[6];
    unsigned char count_0C;
} Item;
extern s32 func_801119B4(Item *item);
extern void func_800D3698(s32 index, s32 value);

s32 func_80111578(Item *item)
{
    s32 count = item->count_0C;
    if (count != 0) {
        s32 index = item->index_05;
        item->count_0C = count - 1;
        if (~index != 0) {
            func_800D3698(index, func_801119B4(item));
        }
        return 1;
    }
    return 0;
}
