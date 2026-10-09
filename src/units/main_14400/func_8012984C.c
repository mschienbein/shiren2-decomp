#include "common.h"
typedef struct { unsigned char pad0[0xBD]; unsigned char fieldBD; } Object;
unsigned char *func_8012984C(Object *object, unsigned char *source) {
    object->fieldBD = *source >> 1;
    return source + 1;
}
