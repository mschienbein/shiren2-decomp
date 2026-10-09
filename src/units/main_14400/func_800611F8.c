#include "common.h"
/* Eight display-list pointers, consumed by func_800610F4. */
extern void *D_80169A18[8];
void func_800611F8(s32 slot, void *display_list) { D_80169A18[slot] = display_list; }
