#include "common.h"
typedef unsigned short u16;
/* Partial view: only the halfword at 0x30 is scaled. */
typedef struct { unsigned char pad_00[0x30]; u16 field_30; } Object;
void func_800E0F0C(Object *object, u16 percent) {
    object->field_30 = object->field_30 * percent / 100;
}
