#include "common.h"

typedef struct {
    char pad0[0x40];
    void *field_40;
} Obj;

/* func_800C4468 stores the hit-unit pointer in this field. */
void *func_800C4834(Obj *obj) {
    return obj->field_40;
}
