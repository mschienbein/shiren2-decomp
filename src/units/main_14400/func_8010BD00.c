#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0xE];
    u8 capacity;
    u8 count;
    u8 items[16];
} ByteList;

s32 func_8010BD00(ByteList *list, u8 item) {
    if (list->count >= list->capacity) {
        return 0;
    }
    list->items[list->count] = item;
    list->count++;
    return 1;
}
