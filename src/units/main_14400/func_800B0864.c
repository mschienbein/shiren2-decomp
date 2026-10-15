#include "common.h"
#include "item_records.h"

typedef unsigned short u16;

/* Record iterator: the pool, then a halfword cursor. Returns the current record (0 when
 * the slot is out of range) and steps past it. */
typedef struct { ItemPool *field_0; u16 count_4; } Obj800B0864;

Item *func_800B0864(Obj800B0864 *obj) {
    return func_800AFD78(obj->field_0, obj->count_4++);
}
