#include "common.h"

/* Item record (12 bytes): the child widget opened by the item sits at +4. */
typedef struct { s32 field0; void *child; s32 field8; } Entry;
typedef struct { char pad[0x50]; Entry *entries; } Object;
/* Widget vtable slot 14 (+0x70/+0x74): child widget of a flattened item index (null = none);
 * func_800957C0 pushes a non-null result on the widget stack. */
void *func_80097614(Object *obj, s32 index) { return obj->entries[index].child; }
