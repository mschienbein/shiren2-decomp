#include "common.h"

/* Vtable D_8015D240 slot 0x18 override (base func_800AF4A8): true for kinds 0x23 and 0x24. */
/* The virtual-call contract supplies self even though this override only tests kind. */
s32 func_8010DDE4(void *self, s32 kind) {
    return (u32)(kind - 0x23) < 2;
}
