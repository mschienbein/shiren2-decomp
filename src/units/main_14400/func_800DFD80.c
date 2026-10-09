#include "common.h"

typedef struct { short field00; short unknown02; void *field04; } Object;
extern char D_80157FA8[], D_80158B68[];
Object *func_800DFD80(Object *object) {
    object->field04 = D_80157FA8;
    object->field00 = 0x36;
    object->field04 = D_80158B68;
    return object;
}
