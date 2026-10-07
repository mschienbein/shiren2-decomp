#include "common.h"

/* Partial view of the animated record through its float at 0x194. */
typedef struct {
    char pad0[0x194];
    float field_194;
} Obj80090C08;

void func_80090C08(Obj80090C08 *obj, float value) {
    obj->field_194 = value;
}
