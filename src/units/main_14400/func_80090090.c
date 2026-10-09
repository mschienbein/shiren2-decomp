#include "common.h"
typedef struct { s32 index; s32 field_04; } Entry;
typedef struct { unsigned char field_00[0x18]; u32 count; Entry *entries; } List;
extern void *func_80091450(u32 size);
extern s32 func_8008DF04(void *stream);
s32 func_80090090(void *stream, List *list)
{
    s32 error = 0;
    s32 bytes = 0;
    u32 i;
    if (list->count) {
        list->entries = func_80091450(list->count * sizeof(Entry));
        if (!list->entries) {
            error = -1;
        } else {
            for (i = 0; i < list->count; i++) {
                list->entries[i].index = func_8008DF04(stream);
                bytes += 4;
            }
        }
    }
    if (error) bytes = -1;
    return bytes;
}
