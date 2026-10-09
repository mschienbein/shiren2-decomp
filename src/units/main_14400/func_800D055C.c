#include "common.h"

typedef struct { unsigned char unknown00[0x70]; short adjust70; short unknown72; void *(*method74)(void *); } VTable;
typedef struct { void *pool_00; VTable *field04; } Child;
typedef struct { unsigned char pad_00[8]; Child **field08; } Object;
extern char D_801545D8[];
void *func_800D055C(Object *object) {
    void *result;
    if (object->field08) {
        Child *child = *object->field08;
        VTable *table = child->field04;
        result = table->method74((char *)child + table->adjust70);
    } else {
        result = D_801545D8;
    }
    return result;
}
