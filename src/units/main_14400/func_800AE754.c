#include "common.h"
typedef short s16;
typedef struct { unsigned char pad_00[0x18]; s16 adjustment_18; s16 reserved_1A; s32 (*query_1C)(void *object, s32 kind); } ItemVTable;
typedef struct { unsigned char pad_00[8]; ItemVTable *vtable_08; } Item;
s32 func_8010C8FC(void *object, s32 add);
/* Both original paths explicitly produce a sign-extended quantity result. */
s16 func_800AE754(void *object, s16 add)
{
    Item *item = object;
    s32 result;
    if (!item->vtable_08->query_1C((unsigned char *)item + item->vtable_08->adjustment_18, 0x1E)) {
        result = 0;
    } else {
        result = (s16)func_8010C8FC(item, add);
    }
    return result;
}
