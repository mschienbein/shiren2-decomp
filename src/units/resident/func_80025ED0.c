#include "common.h"

/* A 32-bit bit view; the original API name and return signedness are unknown. */
u32 func_80025ED0(void) {
    return *(volatile u32 *)0xA450000C;
}
