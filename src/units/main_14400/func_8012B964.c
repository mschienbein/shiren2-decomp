#include "common.h"
typedef struct { unsigned char pad_00[0xCE]; unsigned char field_CE; unsigned char field_CF; unsigned char field_D0; signed char field_D1; unsigned char pad_D2[6]; unsigned char field_D8; } Object;
float func_8012B964(Object *object) {
    if (--object->field_D0 == 0) {
        if (object->field_D1 == 0) {
            object->field_D1 = object->field_D8;
            object->field_D0 = object->field_CE;
        } else {
            object->field_D1 = 0;
            object->field_D0 = object->field_CF;
        }
    }
    return object->field_D1;
}
