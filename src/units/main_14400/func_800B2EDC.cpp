#include "common.h"

typedef unsigned char u8;

/* TU-private views with function-specific names (no shared C++ class identity). Map position
   with a user-declared member-wise copy constructor (the copying func_800B3024.cpp's position
   type also has); rectangle of two positions. */
struct Pos_800B2EDC {
    s32 x;
    s32 y;
    Pos_800B2EDC() {}
    Pos_800B2EDC(const Pos_800B2EDC &other) : x(other.x), y(other.y) {}
};
struct Rect_800B2EDC { Pos_800B2EDC min; Pos_800B2EDC max; };
/* One-byte direction passed by value (big-endian a2 aggregate), as func_800A2594.c receives it. */
struct Dir_800B2EDC { u8 value; Dir_800B2EDC(u8 v) : value(v) {} };

extern "C" {
extern u8 D_80143392;
u32 func_800B1C6C(void *pos);
void *func_800B1F90(void *pos);
void *func_800A2594(Pos_800B2EDC *out, void *from, Dir_800B2EDC direction);
void func_800A30AC(Rect_800B2EDC *out, void *pos, u8 index);
}

static inline Pos_800B2EDC *copy_position(Pos_800B2EDC *out, const Pos_800B2EDC *from) {
    out->x = from->x;
    out->y = from->y;
    return out;
}

/* The unclipped rectangle for a position, written to OUT (returned; the same a0/v0 ABI as a
   rectangle returned by value, which is how func_800B3024 and func_80048B04 call it; those
   declarations are admitted by scratch/omp/rulings/ABI-IDENTICAL-DECLARATIONS.md): floor mode
   N > 0 gives the radius N square, mode 0 gives the room of a 0x1000 tile grown one step
   in directions 3 and 7, otherwise the radius 1 square. A hidden-result (by-value)
   definition cannot latch the result pointer: with otherwise identical source it copies
   a0 at entry (7 words differ). */
extern "C" Rect_800B2EDC *func_800B2EDC(Rect_800B2EDC *out, Pos_800B2EDC *pos) {
    Rect_800B2EDC *dst;
    u8 mode;

    /* Scheduling barrier allowed by the scoped user ruling
       scratch/omp/rulings/DOWHILE0-BARRIER-SCOPED.md (2026-10-14); same entry idiom
       as func_800B310C. This do/while (0) never breaks. GCC 2.8.1 emits loop notes for
       it, and sched.c treats loop notes as a full barrier, so the output latch
       (s3 <- a0) issues after the prologue saves and the position copy, as in the ROM.
       Measured ladder (gxx281pm-gnu291-O2-unsigned, opus-06 receipts): strictly
       compliant best 56 words (cand/t13_B_strict_zero_first_latch.cpp); a
       really-breaking ODD_C block plus the FAKEMATCH store below, 10 words
       (cand/t09_B_rb1_pod.cpp); with this barrier, 0 words. Without the barrier the
       prologue order differs (5 words). */
    do {
        dst = out;
    } while (0);
    /* FAKEMATCH: dead store. The mode is re-read on the next line, so flow deletes this
       guarded default and it leaves no instruction. Its branch survives until jump2,
       where its label stops find_equiv_reg from treating the nonzero-mode `move a1,s0`
       as a no-op, so that restore stays; jump2 then deletes the empty branch.
       Alternatives measured: without it, or with an unconditional default, the restore
       is deleted (324 bytes, 70 words); placed before the barrier, 332 bytes and 81 words. */
    if (pos == 0) mode = 0;
    mode = D_80143392;
    /* Declared after the mode read: the address of AREA (the implicit constructor's
       `this`) becomes a pseudo after the barrier and fills the mode branch delay slot. */
    Rect_800B2EDC area;
    if (mode) {
        func_800A30AC(&area, pos, mode);
    } else if (func_800B1C6C(pos) & 0x1000) {
        Rect_800B2EDC grown;
        Pos_800B2EDC offset, point;
        Rect_800B2EDC *room = (Rect_800B2EDC *)func_800B1F90(pos);
        func_800A2594(&offset, copy_position(&point, &room->min), Dir_800B2EDC(3));
        grown.min = offset;
        func_800A2594(&offset, copy_position(&point, &room->max), Dir_800B2EDC(7));
        grown.max = offset;
        area.min = grown.min;
        area.max = grown.max;
    } else {
        func_800A30AC(&area, pos, 1);
    }
    dst->min.x = area.min.x;
    dst->min.y = area.min.y;
    dst->max.x = area.max.x;
    dst->max.y = area.max.y;
    return dst;
}
