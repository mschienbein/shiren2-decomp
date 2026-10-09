#include "common.h"
typedef struct { unsigned char fields00[0x44]; unsigned char field44; } Object;
extern u32 func_800E1148(const Object *object);
s32 func_800E1D14(Object *object, s32 value) {
    s32 result = 0;
    if (object->field44 != 0) {
        result = func_800E1148(object) == value;
    }
    return result;
}
