#include "func_801EC818_13E738/types.h"

extern SelectionRecord D_80142F18;
u8 func_801E8CF4_13AC14(u8, u8);

static inline s32 selection_variant(void) {
    return D_80142F18.variant;
}

void func_801EC818_13E738(void) {
    s32 kind = 0;

    switch (selection_variant()) {
    case 0:
        kind = 1;
        break;
    case 7:
        kind = 2;
        break;
    case 10:
        kind = 3;
        break;
    }
    func_801E8CF4_13AC14(10, kind);
}
