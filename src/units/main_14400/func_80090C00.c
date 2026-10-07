#include "common.h"

/* Partial view of the animated record through its float at 0x190. */
typedef struct {
    char pad0[0x190];
    float field_190;
} Obj;

void func_80090C00(Obj *obj, float value) {
    obj->field_190 = value;
}
