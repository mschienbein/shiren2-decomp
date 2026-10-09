#include "common.h"
typedef struct { unsigned char pad_0[0xE4]; unsigned short field_E4; } Object;
void func_800EE084(Object *object) { object->field_E4 &= ~0x100; }
