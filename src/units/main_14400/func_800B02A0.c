#include "common.h"
#include "item_records.h"

extern void func_800B0054(void *table, void *stream);

void func_800B02A0(void *stream)
{
    s32 index = 1;
    do {
        ItemPool *pool = D_80153AE4[index++];
        func_800B0054(pool, stream);
    } while (index < 7);
}
