#include "common.h"

/*
 * Widget vtable slot 14 (+0x70 this-adjust / +0x74 method), bound at
 * D_80152620+0x74: returns the child widget for a flattened item index
 * (null = none). The menu driver func_800957C0 passes the adjusted self
 * and the index and consumes the result as an object pointer; this class
 * has no children, so both parameters are unused.
 */
void *func_8009ACD8(void *self, s32 index)
{
    return 0;
}
