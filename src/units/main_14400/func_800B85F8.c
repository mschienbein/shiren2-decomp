#include "common.h"

extern s32 func_80049CB4(s32 command, ...);

/* The map virtual slot supplies a receiver, unused by this command-only method. */
void func_800B85F8(void *object) {
    func_80049CB4(5, 1);
}
