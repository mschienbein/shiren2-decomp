#include "func_801E58A0_1377C0/types.h"

void func_8006AAF0(void *dst, u32 devAddr, s32 size);
extern u8 D_0023E210[];

/* src is one device-address pointer reused for the three DMA/index addresses:
 * the ROM table base, the staged index entry in RAM, and the ROM record block.
 * Only ROM pointers are converted to u32 DMA addresses; the RAM index pointer is
 * never carried through an integer. Reusing src keeps CSE from holding
 * D_0023E210 in a callee-saved register across the first DMA (ROM rematerializes
 * it at 801E58B4 and 801E58F0).
 *
 * ROM bounds are retained, not justified by a larger C object: the staging
 * buffer is 0x190 bytes, but key * 8 and the 128-record scan can exceed it.
 * Known callers (func_801F1914 key 0, func_801F1FF4 key 1, via func_801E5B50)
 * use index entries at ROM 0x23E210/0x23E218 (offsets 0x10, 0x1AF0), which
 * give skip 0 and scans that stop at records 19/24; entries 2..47 of the
 * staged 0x180 bytes are record data, not index entries. That bounds only the
 * known direct routes, not every entry or DMA/transfer state. */
void func_801E58A0_1377C0(void) {
    u8 *buf = D_801F6550_148470;
    u8 key = D_801F6710_148630->x2;
    u8 *src;
    D_801F6550_index *index;
    D_801F6550_record *record;
    u32 addr;
    u32 aligned;
    s32 skip;
    u32 i;

    src = D_0023E210;
    func_8006AAF0(buf, (u32)src, 0x180);
    src = buf + key * 8;
    index = (D_801F6550_index *)src;
    src = D_0023E210 + index->offset;
    addr = (u32)src;
    skip = addr & ~7;
    aligned = skip;
    skip = addr - skip;
    D_801F6710_148630->xC = index->x4;
    func_8006AAF0(buf, aligned, 0x190);
    if (skip != 0) {
        /* ORIGINAL BUG: partially overlapping struct assignment, undefined in
         * ISO C. skip = addr - (addr & ~7) is 1..7 here for any key. The pinned
         * GCC 2.8.1 movstrsi expansion tests (src|dst)&3 and copies 24 forward
         * 16-byte chunks (lwl/lwr+swl/swr 801E593C..801E5988, lw/sw
         * 801E59B0..801E59DC); each chunk loads four words before storing four
         * and the destination is below the source, so stored bytes never reach
         * unread source bytes: buf[0..0x17F] = old buf[skip..skip+0x17F], and
         * buf[0x180..0x18F] keep the DMA data. See
         * scratch/omp/rulings/ORIGINAL-OVERLAP-COPY.md. */
        *(D_801F6550_block *)buf = *(D_801F6550_block *)(buf + skip);
    }
    record = (D_801F6550_record *)buf;
    for (i = 0; i < 128; i++, record++) {
        if (record->key == 0) {
            D_801F6710_148630->x8 = record->value;
            D_801F6710_148630->xA = record->value;
            return;
        }
        if (record->key == D_801F6710_148630->x0) {
            break;
        }
    }
    D_801F6710_148630->x8 = record[0].value;
    D_801F6710_148630->xA = record[1].value;
}
