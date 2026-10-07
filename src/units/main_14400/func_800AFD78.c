#include "common.h"

typedef unsigned char u8;
typedef struct { char data[0x30]; } Item;
typedef struct {
    Item *items;
    s32 field_4;
    s32 count;
} Table;

Item *func_800AFD78(Table *table, u8 index) {
    if (index == 0xFF) {
        return 0;
    }
    if (index >= table->count) {
        return 0;
    }
    return &table->items[index];
}
