#include "common.h"
#include "item_records.h"
void *func_800AC5B4(s32 size, s32 alternate);
Item *func_80123990(Item *object);
Item *func_801239D4(void)
{
    return func_80123990(func_800AC5B4(0x10, 0));
}
