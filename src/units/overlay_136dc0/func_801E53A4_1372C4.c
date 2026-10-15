#include "func_801E53A4_1372C4/types.h"

u8 func_801E53A4_1372C4(s32 pos) {
    s32 block = pos / 384;
    s32 offset;

    if (D_801F6710_148630 != &D_801F66E0_148600[0]) {
        D_801F6710_148630 = &D_801F66E0_148600[0];
        D_801F66E0_148600[0].x4 = block;
        D_801F6710_148630->x6 = func_801E52EC_13720C();
    } else if (D_801F6710_148630->x4 != block) {
        D_801F6710_148630->x4 = block;
        D_801F6710_148630->x6 = func_801E52EC_13720C();
    }
    offset = pos % 384;
    return D_801F6550_148470[offset + D_801F6710_148630->x6];
}
