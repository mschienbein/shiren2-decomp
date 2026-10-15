#include "common.h"
#include "item_records.h"
void func_800AF910(ItemPool *pool, s32 used) { pool->primaryCount = pool->capacity - used; }
