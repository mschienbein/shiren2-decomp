#include "common.h"

typedef short s16;
typedef struct Obj800F414C Obj800F414C;

/* Empty override of the actor step slot (vtable +0x78/+0x7C): the dispatcher
 * (func_800F414C, func_800F2598) passes the adjusted receiver and a signed
 * halfword step; this implementation ignores both. */
void func_800EFC04(Obj800F414C *self, s16 step) {
}
