#include "common.h"

/* Owner prefix through the retained actor pointer at +0x104. */
typedef struct {
    unsigned char pad0[0x104];
    void *field_104;
} Obj800EE290;

void *func_800EE290(Obj800EE290 *self) {
    return self->field_104;
}
