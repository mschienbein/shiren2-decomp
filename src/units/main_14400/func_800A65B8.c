#include "common.h"
typedef struct Object Object;
typedef struct Value Value;
extern s32 func_800A650C(Object *, Value *);
s32 func_800A65B8(Object *object, void *value) {
    s32 distance;
    if (value) distance = func_800A650C(object, value);
    else distance = 0x4C;
    return distance;
}
