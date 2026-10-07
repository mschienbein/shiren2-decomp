#include "common.h"

/* The callee reads CP0 Count; callers store or pass the returned 32-bit value. */
extern u32 func_8002A9B0(void);

u32 func_800413C0(void)
{
    return func_8002A9B0();
}
