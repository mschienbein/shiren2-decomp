#include "common.h"
#include "item_records.h"
extern s32 func_800AF28C(Item *, s32 *);
s32 func_80127CC0(Item *a, s32 *b) { if (*b == 0x1B) { if (a) a->field08->method0C((char *)a + a->field08->delta08, 3); return 1; } else return func_800AF28C(a, b); }
