#include "common.h"
typedef struct { void *field_0, *field_4; } Pair;
extern s32 func_800CD2BC(void *, void *);
void *func_800D02AC(Pair *object) {
    void *value = object->field_4;
    if (func_800CD2BC(object->field_0, value)) {
        object->field_4 = 0;
        return value;
    }
    return 0;
}
