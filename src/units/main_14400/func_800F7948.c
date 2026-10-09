#include "common.h"
typedef struct { unsigned char pad00[0x1E]; unsigned char flags1E; } Item;
typedef struct { unsigned char pad00[0xA4]; s32 fieldA4; } Object;
void func_800F7948(Object *object, Item *item) {
    if (item != 0 && ((item->flags1E >> 2) & 1)) object->fieldA4 = 1;
}
