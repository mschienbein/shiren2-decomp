#include "common.h"
s32 func_800A41D0(s32 flags, s32 mode) {
    switch (mode) {
    case 2: return (flags & 0x8020) == 0;
    default: return 0;
    }
}
