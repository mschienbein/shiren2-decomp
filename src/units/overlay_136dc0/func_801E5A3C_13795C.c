#include "func_801E5A3C_13795C/types.h"

void func_8006AAF0(void *dst, u32 devAddr, s32 size);
extern u8 D_0023E210[];

/* Reloads the 0x190-byte staging window for the active cursor: ROM address
 * D_0023E210 + xC + x4 * 384, rounded down to 8 for the DMA. xC is written only
 * by func_801E58A0_1377C0 (sw 801E5920, from the index entry's second word), and
 * 0x23E210 and 384 are multiples of 8, so skip == xC & 7 for every block.
 * Known routes: index 0 (func_801F1914) has xC 0x60, skip 0; index 1
 * (func_801F1FF4) has xC 0x1B54, skip 4, so the move below runs on that path. */
void func_801E5A3C_13795C(void) {
    u8 *buf = D_801F6550_148470;
    u32 addr;
    u32 aligned;
    s32 skip;

    addr = D_801F6710_148630->xC + (u32)D_0023E210;
    addr += D_801F6710_148630->x4 * 384;
    skip = addr & ~7;
    aligned = skip;
    skip = addr - skip;
    func_8006AAF0(buf, aligned, 0x190);
    if (skip != 0) {
        /* ORIGINAL BUG: partially overlapping struct assignment, undefined in
         * ISO C. skip is 1..7 here (4 on the index-1 route). The pinned
         * GCC 2.8.1 movstrsi expansion tests (src|dst)&3 and copies 24 forward
         * 16-byte chunks (lwl/lwr+swl/swr 801E5ABC..801E5B04, lw/sw
         * 801E5B10..801E5B38; skip 4 takes the lw/sw path because buf is
         * 16-aligned). Each chunk loads four words before storing four and the
         * destination is below the source, so no unread source byte is
         * overwritten: buf[0..0x17F] = old buf[skip..skip+0x17F], and
         * buf[0x180..0x18F] keep the DMA data. See
         * scratch/omp/rulings/ORIGINAL-OVERLAP-COPY.md. */
        *(D_801F6550_block *)buf = *(D_801F6550_block *)(buf + skip);
    }
}
