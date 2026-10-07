#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0;
    u8 flags_1;
} Obj_80090E44;

void func_80090E44(Obj_80090E44 *obj, float value) {
    if (value != 0.0f) {
        obj->flags_1 |= 1;
    } else {
        obj->flags_1 &= ~1;
    }
}
