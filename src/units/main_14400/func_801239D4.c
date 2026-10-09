#include "common.h"
typedef struct Item800F1568 Item800F1568;
void *func_800AC5B4(s32 size, s32 alternate);
Item800F1568 *func_80123990(Item800F1568 *object);
Item800F1568 *func_801239D4(void)
{
    return func_80123990(func_800AC5B4(0x10, 0));
}
