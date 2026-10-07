#include "common.h"

typedef struct Obj800F414C Obj800F414C;

/* Empty override of the actor add slot (vtable +0x80/+0x84): the dispatcher
 * (func_800F414C, func_800F2598) passes the adjusted receiver and a full-width
 * amount; this implementation ignores both. */
void func_8010B898(Obj800F414C *self, s32 amount) {
}
