#include "common.h"

/* Widget vtable slot 14 (+0x70/+0x74, bound in D_80152000, D_80152098 and
 * D_80152130): returns the child widget for a flattened item index, null for
 * none. This class has no child widgets; self and index are passed by the
 * slot contract and left unused. */
void *func_80096CB0(void *self, s32 index)
{
    return 0;
}
