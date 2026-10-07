#include "common.h"

typedef struct {
    char pad0[8];
    s32 unk8;
} Inner800D4A10;

s32 func_800D4A10(Inner800D4A10 **arg0) {
    Inner800D4A10 *inner = *arg0;

    if (inner == 0) {
        return 0;
    }
    return inner->unk8;
}
