#include "common.h"
typedef struct { unsigned char pad_00[0x9A]; unsigned short field_9A; } Object;
void func_800F3B20(Object *object) {
    object->field_9A |= 0x10;
}
