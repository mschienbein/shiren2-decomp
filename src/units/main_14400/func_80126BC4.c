#include "common.h"
typedef struct { unsigned char pad00[0x10]; unsigned char field10; } Object;
unsigned char *func_80126BC4(unsigned char *result, Object *object) {
    *result = object->field10;
    return result;
}
