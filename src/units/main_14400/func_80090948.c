#include "common.h"
typedef struct { u32 index; s32 field_04; } Entry;
typedef struct { unsigned char field_00[0x18]; u32 count; Entry *entries; } List;
typedef struct { s32 field_00[2]; void **entries; } Table;
typedef struct { unsigned char field_00[0x74]; Table *table; } Context;
typedef struct { unsigned char field_00[0x1B4]; void *field_1B4; } Owner;
extern s32 func_8008D4B8(void *owner, void *entry, s32 mode);
s32 func_80090948(void *data, Context *context, Owner *owner)
{
    u32 i = 0;
    List *list = data;
    s32 result = 0;
    for (; i < list->count; i++) {
        result = func_8008D4B8(owner->field_1B4, context->table->entries[list->entries[i].index], 2);
        if (result) break;
    }
    return result;
}
