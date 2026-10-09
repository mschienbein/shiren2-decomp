#include "common.h"

typedef struct {
    char pad0[0xC];
    unsigned char flags_C;
} Obj_80116B24;

void func_80116B24(Obj_80116B24 *obj) {
    obj->flags_C |= 4;
}
