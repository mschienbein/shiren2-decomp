#include "common.h"

typedef struct {
    unsigned char field_00;
} Obj;

s32 func_8009A2D8(Obj *obj) {
    return (u32)(obj->field_00 - 3) < 2;
}
