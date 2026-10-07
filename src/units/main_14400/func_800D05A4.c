#include "common.h"

typedef struct { void *container; void *item; } Pair;
typedef struct { char pad0[0x8]; Pair *items; s32 capacity; s32 count; } List;

void func_800D05A4(List *list, Pair *item) {
    s32 count = list->count;

    if (count < list->capacity) {
        list->items[count] = *item;
        list->count = count + 1;
    }
}
