#include "common.h"
typedef struct Reader Reader;
typedef struct { unsigned char pad_00[0x18]; s32 count_18; void **entries_1C; } List;
extern void *func_8008F76C(Reader *first, s32 second);
s32 func_8008FA90(Reader *first, s32 second, List *list) {
    s32 result = 0;
    void *entry = func_8008F76C(first, second);
    if (entry == 0) result = -1;
    else { list->entries_1C[list->count_18++] = entry; }
    return result;
}
