#include "common.h"

typedef struct {
    unsigned char pad_00[0x10];
    s32 field_10;
    s32 field_14;
} Obj;

void func_8011BBB8(Obj *obj) {
    obj->field_10 = 0;
    obj->field_14 = 0;
}
