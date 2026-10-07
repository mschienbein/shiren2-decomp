#include "common.h"
extern void func_8009B7CC(void *object);
extern void func_8009B8F8(void *object);
typedef struct { unsigned char pad_00[0x80]; s32 field_80; } Object;
void func_8009B790(Object *object) {
    if (object->field_80 == 3) func_8009B7CC(object);
    else func_8009B8F8(object);
}
