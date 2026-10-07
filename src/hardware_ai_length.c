#include "common.h"

/* Original name pending symbol research. The hardware register is AI_LEN. */
s32 func_80025EC0(void) {
    return *(volatile s32 *)0xA4500004;
}
