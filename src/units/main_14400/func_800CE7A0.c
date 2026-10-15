#include "common.h"
#include "item_records.h"

/* Item list: pool +0, entry buffer +8, count +0xE. */
typedef struct {
    ItemPool *field_0;
    unsigned char pad4[4];
    unsigned char *field_8;
    unsigned char padC[2];
    unsigned char count;
} List;

/* Entry `index` of the list (generic result, consumed under many views), or 0. */
void *func_800CE7A0(List *list, u32 index)
{
    if (index < list->count) {
        return func_800AFD78(list->field_0, list->field_8[index]);
    }
    return 0;
}
