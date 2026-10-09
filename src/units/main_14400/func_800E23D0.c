#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad00[0x54];
    u8 flags54;
} Obj800E23D0;

void func_800E23D0(Obj800E23D0 *obj)
{
    obj->flags54 |= 1;
}
