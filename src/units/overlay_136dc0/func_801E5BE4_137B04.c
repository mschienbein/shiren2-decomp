#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

/* D_801F66E0_148600[3]: 16-byte streaming cursors; D_801F6710_148630 points at the active one
 * (view as in src/units/overlay_136dc0/func_801E5B50_137A70/types.h). */
typedef struct {
    u8 x0;
    u8 unk1;
    u16 x2;
    u16 x4;    /* current 384-byte block of the staging window */
    u16 x6;
    u16 x8;
    u16 xA;
    u32 xC;
} D_801F66E0_view;
extern D_801F66E0_view *D_801F6710_148630;
/* 0x190-byte ROM staging buffer reloaded by func_801E5A3C_13795C.
 * ORIGINAL BUG: the first copy in func_801E5BE4_137B04 below reads past the
 * end of this array for reachable records: index 122 reads 0x801F66E0..E1 and
 * index 157 reads 0x801F66E0..E3, the first bytes of the next object,
 * D_801F66E0_148600[0] (x0, unk1, x2). The extent stays the evidenced 0x190:
 * no DMA into it exceeds 0x190 bytes (0x190 at 801E5358, 801E5918 and
 * 801E5A5C), the accepted ELF starts D_801F66E0_148600 at 0x801F66E0, and no
 * reader declares an enclosing object.
 * Ruling: scratch/omp/rulings/ORIGINAL-BUG-OVERREAD.md. Evidence:
 * scratch/omp/wave39/opus-hardcore-matching/opus-r6b/overread-5be4/. */
extern u8 D_801F6550_148470[0x190];

void func_801E5A3C_13795C(void);
/* ABI-identical declaration admitted by
 * scratch/omp/rulings/ABI-IDENTICAL-DECLARATIONS.md (user ruling 2026-10-14,
 * scoped debt, this file and symbol only). The definition of libultra memcpy
 * func_80032D94 in src/units/resident/func_80032D30.c returns void * (its dst).
 * Here it is declared returning void. The ABI is identical: arguments are in
 * a0..a2 either way, the callee still returns dst in $v0, and both calls below
 * ignore that value, so no caller reads $v0.
 * Measured mechanism: with the void * declaration each call is a call_value
 * that sets $v0. flow.c therefore marks $v0 in regs_ever_live, reload1.c:582
 * copies it into regs_explicitly_used, and order_regs_for_reload
 * (reload1.c:3894) puts it in bad_spill_regs = forbidden_regs (reload1.c:830).
 * When reload spills HI for the index * size multiply (insn 14), the HI half
 * of the /384 multiply (pseudo 85) is re-placed by retry_global_alloc
 * (global.c:1206) outside $v0, in $v1: mfhi $v1 instead of the ROM's
 * mfhi $v0 at 0x801E5C28, and the D_801F6710 loads shift.
 * Word counts: void * declaration 13 differing words
 * (opus-05/overlay-r2/ladder-5be4: 19 beam candidates and 135 scored permuter
 * candidates found nothing better); void declaration 0 words. Dumps:
 * overlay-r2/ladder-5be4/result/baseline-explain (greg: "Register 85 now in 3")
 * versus overlay-r2/diag-5be4-void (greg: "Register 85 now in 2"). */
void func_80032D94(void *dst, const void *src, u32 size);

/* Copies record `index` (`size` bytes each) of the streamed ROM table into dst.
 * The table is read through the 384-byte staging window of the active cursor;
 * a record that crosses the window end is finished from the next block. */
void func_801E5BE4_137B04(u32 index, u32 size, u8 *dst) {
    u32 pos = index * size;
    u32 block = pos / 384;
    u32 offset = pos % 384;

    if (block != D_801F6710_148630->x4) {
        D_801F6710_148630->x4 = block;
        func_801E5A3C_13795C();
    }
    /* ORIGINAL BUG: the ROM copies the whole record here unconditionally
     * (jal at 801E5C5C; a2 = size, kept in s2 since 801E5BEC), so a record
     * with offset + size > 0x190 is read past the end of the window. Reached
     * through 801F1914 -> 801E5B50 -> 801F1A00: event records (size 0x16) of
     * ROM table 0x23E210, key 0x61, indices [112,181). Index 122 (offset 380)
     * reads 0x801F66CC..0x801F66E1, 2 bytes past the end; index 157 (offset
     * 382) reads 0x801F66CE..0x801F66E3, 4 bytes past the end
     * (D_801F66E0_148600[0].x0, .unk1 and .x2). Those bytes land in
     * dst[0x190 - offset, size), inside dst[384 - offset, size), which the
     * second copy below rewrites after func_801E5A3C_13795C (which never reads
     * dst) reloads the window, so the values read are discarded. Six safe
     * guarded forms miss by 50-55 differing words plus 4-9 extra words
     * (forms-result.json in the evidence directory named at the
     * D_801F6550_148470 declaration); G8 proof
     * scratch/omp/wave39/lean-r6/g8-independent/copy-overread-proof.json. */
    func_80032D94(dst, D_801F6550_148470 + offset, size);
    if (offset + size >= 384) {
        u32 head = 384 - offset;
        u32 rest = size - head;

        D_801F6710_148630->x4 = block + 1;
        func_801E5A3C_13795C();
        func_80032D94(dst + head, D_801F6550_148470, rest);
    }
}
