#include "common.h"

/* +0x100 retains the item/object pointer that func_800EBFE4 obtains from
 * func_800E215C; the item stays opaque here. */
typedef struct { char pad[0x100]; void *item; } S;
void *func_800EE258(S *s) { return s->item; }
