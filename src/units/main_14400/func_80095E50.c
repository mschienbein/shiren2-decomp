#include "common.h"

/* Widget vtable slot 8 (+0x40 this-adjust, +0x44 method): handles an input event code;
 * func_80046CB0 passes the code and dispatches on the result. The default ignores the
 * event (self and event are passed by the slot contract and unused) and returns -1. */
s32 func_80095E50(void *self, s32 event)
{
    return -1;
}
