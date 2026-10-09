#include "common.h"

typedef struct {
    char pad0[0x18];
    void *field_18;
} Obj;

/* func_800CFF00 initializes this field from its owner pointer. */
void *func_800D015C(Obj *obj) {
    return obj->field_18;
}
