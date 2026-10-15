#include "common.h"
#include "item_records.h"
void func_800AF8D0(ItemPool *pool) { unsigned char *p=pool->occupancy; s32 count=(pool->capacity+7)/8; while(count-- > 0) *p++=0; D_80143104=0; }
