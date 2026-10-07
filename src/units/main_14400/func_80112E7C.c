#include "common.h"
extern s32 func_80112EE4(void *object);
typedef struct { unsigned char pad_00[0xC]; unsigned char field_0C; } Object;
void func_80112E7C(Object *object) {
    func_80112EE4(object);
    object->field_0C |= 1;
}
