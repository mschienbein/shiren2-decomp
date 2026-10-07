#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 active;
    char pad1[0x1C - 0x1];
    u32 count;
    u32 capacity;
    void **items;
} List80070598;

extern s32 D_8013D454;

s32 func_80070598(void *object, void *value) {
    List80070598 *list = object;
    u32 count;

    if (D_8013D454 == 0 || list->active == 0) {
        return -1;
    }
    count = list->count;
    if (count >= list->capacity) {
        return -1;
    }
    list->items[count] = value;
    list->count = count + 1;
    return 0;
}
