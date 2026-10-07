#include "common.h"

typedef struct {
    s32 pad0;
    s32 field_4;
} Obj800D3648;

void func_800D3648(Obj800D3648 *obj, s32 value) {
    obj->field_4 = value;
}
