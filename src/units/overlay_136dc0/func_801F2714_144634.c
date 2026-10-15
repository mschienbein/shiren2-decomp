#include "func_801F2714_144634/types.h"

static inline u8 table_get(s32 index) {
    return D_801F5228_147148[index];
}

static inline s32 current_mode(void) {
    return D_80142F18.mode;
}

s32 func_801F2714_144634(void) {
    s32 count = table_get(8);

    func_801E8CF4_13AC14(8, count + 1);
    switch (current_mode()) {
    case 0x61:
    case 0x66:
    case 0x70:
        break;
    default:
        return 0;
    }
    if (table_get(0) < 5) {
        return 0;
    }
    if (count % 24 == 0) {
        func_801F2254_144174(0x46C, 0);
        return 1;
    }
    return 0;
}
