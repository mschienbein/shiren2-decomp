#include "common.h"

/* Partial view of the animated record through its float at 0x17C. */
typedef struct {
    char pad0[0x17C];
    float field_17C;
} Obj80090BD8;

void func_80090BD8(Obj80090BD8 *obj, float value) {
    obj->field_17C = value;
}
