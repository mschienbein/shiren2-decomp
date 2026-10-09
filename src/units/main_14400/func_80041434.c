#include "common.h"

/* func_80061230 swaps the flag and returns the previous value (lw v0 before the
 * delay-slot store); this caller discards it. */
extern s32 func_80061230(s32 flag);

void func_80041434(s32 value) {
    func_80061230(value != 0);
}
