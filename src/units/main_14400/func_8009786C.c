#include "common.h"

typedef struct {
    char pad0[0x20];
    short delta_20;
    short index_22;
    void (*func_24)(char *self);
} VTable8009786C;

typedef struct {
    char pad0[0x4C];
    VTable8009786C *vtable_4C;
    char pad50[8];
    s32 value_58;
    s32 max_5C;
    s32 min_60;
    s32 digits_64;
    s32 digit_68;
} Obj8009786C;

void func_8009786C(Obj8009786C *obj, s32 action) {
    s32 step;
    s32 i;

    switch (action) {
    case 0:
        if (obj->value_58 > obj->min_60) {
            step = 1;
            for (i = 0; i < obj->digit_68; i++) {
                step *= 10;
            }
            obj->value_58 -= step;
            obj->value_58 = (obj->value_58 < obj->min_60) ? obj->min_60 : obj->value_58;
        }
        break;
    case 1:
        if (obj->value_58 < obj->max_5C) {
            step = 1;
            for (i = 0; i < obj->digit_68; i++) {
                step *= 10;
            }
            obj->value_58 += step;
            obj->value_58 = (obj->value_58 > obj->max_5C) ? obj->max_5C : obj->value_58;
        }
        break;
    case 2:
        if (obj->digit_68 > 0) {
            obj->digit_68--;
        }
        break;
    case 3:
        if (obj->digit_68 < obj->digits_64 - 1) {
            obj->digit_68++;
        }
        break;
    }
    obj->vtable_4C->func_24((char *)obj + obj->vtable_4C->delta_20);
}
