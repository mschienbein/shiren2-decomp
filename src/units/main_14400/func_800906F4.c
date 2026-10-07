#include "common.h"

typedef unsigned char u8;

typedef struct { u8 pad0[2]; u8 count; u8 pad3[5]; void **items; } List;
/* reader is the stream object func_800905DC forwards to func_8008DF04/func_8008E0C4. */
void *func_800905DC(void *reader, s32 size);
s32 func_800906F4(void *reader, s32 size, List *list) {
    s32 result = 0;
    void *item = func_800905DC(reader, size);
    if (item == 0) {
        result = -1;
    } else {
        list->items[list->count++] = item;
    }
    return result;
}
