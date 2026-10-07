#include "common.h"

/* Observed callers dereference this external word as a record address.
 * The historical return type and complete record type remain unresolved.
 */
extern void *D_800373C0;

void *func_80033D20(void)
{
    return D_800373C0;
}
