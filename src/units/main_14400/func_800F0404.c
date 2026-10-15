#include "common.h"
#include "item_records.h"
/* The owner's held id-0xAC pot item (func_800F0314), or 0. */
extern Item *func_800F0314(void *object);
extern s32 func_80121848(void *container, void *object);
/* Both original paths produce a boolean; the prior void declaration loses it. */
s32 func_800F0404(void *object) {
    Item *container = func_800F0314(object);
    s32 result;
    if (!container) result = 0;
    else result = func_80121848(container, object);
    return result;
}
