#include "common.h"

typedef unsigned char u8;

/* TU-private views with function-specific names (no shared C++ class identity). Map position
   with a user-declared member-wise copy constructor (the copying func_800B3024.cpp's position
   type also has); rectangle of two positions. */
struct Pos_800B310C {
    s32 x;
    s32 y;
    Pos_800B310C() {}
    Pos_800B310C(const Pos_800B310C &other) : x(other.x), y(other.y) {}
};
struct Rect_800B310C { Pos_800B310C min; Pos_800B310C max; };

extern "C" {
extern u8 D_80143392;
extern Rect_800B310C D_801429C0;
u32 func_800B1C6C(void *pos);
void *func_800B1F90(void *pos);
void func_800A30AC(Rect_800B310C *out, void *pos, u8 index);
void func_800A3448(s32 *area, s32 *bounds);
}

/* The rectangle for a position, clipped to the map bounds and written to OUT (returned):
   floor mode N > 0 gives the radius N-1 square, mode 0 gives the room of a 0x1000 tile,
   otherwise the single-tile square. */
extern "C" Rect_800B310C *func_800B310C(Rect_800B310C *out, Pos_800B310C *pos) {
    Rect_800B310C *dst;
    u8 mode;

    /* Scheduling barrier allowed by the scoped user ruling
       scratch/omp/rulings/DOWHILE0-BARRIER-SCOPED.md (2026-10-14). This do/while (0)
       never breaks. GCC 2.8.1 emits loop notes for it, and sched.c treats loop notes as a
       full barrier, so the output latch (s2 <- a0) issues after the prologue saves and
       the position copy (s0 <- a1), just before the floor-mode load, as in the ROM.
       Measured ladder (opus-06 receipts, gxx281pm-gnu291-O2-unsigned except where noted):
       strictly compliant best 28 words (cand/t10_strict_zero_first_break_latch.cpp); a
       really-breaking ODD_C block plus the FAKEMATCH store below, 10 words
       (cand/t01_rb1_whole_selection_breaks.c, a C variant measured under
       gcc281pm-gnu291-O2-unsigned); with this barrier, 0 words. Without the barrier the
       prologue order differs (4 words). */
    do {
        dst = out;
    } while (0);
    /* FAKEMATCH: dead store. The mode is re-read on the next line, so flow deletes this
       guarded default and it leaves no instruction. Its branch survives until jump2,
       where its label stops find_equiv_reg from treating the nonzero-mode `move a1,s0`
       as a no-op, so that restore stays; jump2 then deletes the empty branch.
       Alternatives measured: without it, or with an unconditional default, the restore
       is deleted (216 bytes, 43 words); placed before the barrier, 224 bytes and 54 words. */
    if (pos == 0) mode = 0;
    mode = D_80143392;
    /* Declared after the mode read: the address of AREA (the implicit constructor's
       `this`) becomes a pseudo after the barrier and fills the mode branch delay slot. */
    Rect_800B310C area;
    if (mode) {
        func_800A30AC(&area, pos, mode - 1);
    } else if (func_800B1C6C(pos) & 0x1000) {
        area = *(Rect_800B310C *)func_800B1F90(pos);
    } else {
        func_800A30AC(&area, pos, 0);
    }
    func_800A3448((s32 *)&area, (s32 *)&D_801429C0);
    dst->min.x = area.min.x;
    dst->min.y = area.min.y;
    dst->max.x = area.max.x;
    dst->max.y = area.max.y;
    return dst;
}
