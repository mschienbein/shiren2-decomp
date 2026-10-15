#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct { u8 x, y; } BytePair;

/* D_801F6714_148634[10]: 8-byte timed sound/event slots (view as in
 * src/units/overlay_136dc0/func_801E5FB4_137ED4/types.h). */
typedef struct {
    u8 x0;     /* period - 1 */
    u8 x1;     /* tick counter */
    u8 x2;
    u8 x3;
    u8 x4;
    u8 x5;     /* repeats left, 0xFF = forever */
    u16 x6;    /* id, 0xFFFF = free */
} D_801F6714_view;
extern D_801F6714_view D_801F6714_148634[10];

void func_801E5FB4_137ED4(void);

/* Starts timed slot `id` in the first free D_801F6714_148634 entry, searching
 * from the top; id 0xFFFF clears every slot instead. `repeat` 0xFF repeats
 * forever and makes the first tick fire at once (counter = period). A finite
 * count starts the counter from 0. */
void func_801E6000_137F20(u16 id, s32 period, s32 x2, BytePair *pair, u8 repeat) {
    s32 i = 10;
    D_801F6714_view *slot = D_801F6714_148634 + i;

    if (id == 0xFFFF) {
        func_801E5FB4_137ED4();
        return;
    }
    while (--i != -1) {
        --slot;
        if (slot->x6 == 0xFFFF) {
            slot->x6 = id;
            slot->x0 = period - 1;
            slot->x2 = x2;
            slot->x1 = period;
            slot->x3 = pair->x;
            slot->x4 = pair->y;
            slot->x5 = repeat;
            /* FAKEMATCH: the else arm re-stores the period that was already
             * written to x1 above. It changes no memory and exists only for
             * codegen; the original has no second period store. The real
             * stores are the initial x1 = period at ROM 0x801E6074 and the
             * finite-repeat x1 = 0 at 0x801E6038 (delay slot of the j at
             * 0x801E6034, the exit block placed in front of the loop). The
             * infinite-repeat path reaches j/nop at 0x801E6090/0x801E6094
             * with no store.
             * Failed alternatives (scratch/omp/match/func_801E6000_137F20/):
             * - no else arm (opus-05 overlay/c/e6000_i.c): policy, 168 B vs
             *   176 B, 31 words (20261014T131912-dsl7l9f8);
             * - ternary x1 = repeat != 0xFF ? 0 : period
             *   (c/e6000_probe_tern_early.c): 176 B, 3 words
             *   (20261014T134244-vha52oqj);
             * - x1 stored only after the pair reads (c/e6000_probe_ifelse_x1.c):
             *   policy, 172 B, 17 words.
             * [INFERENCE] Not observed in RTL dumps: without the else store,
             * jump.c threads the repeat test straight to the return, so
             * loop.c neither moves the x1 = 0 exit block nor bases the slot
             * giv on x1; a later pass deletes the redundant store. */
            if (repeat != 0xFF) {
                slot->x1 = 0;
            } else {
                slot->x1 = period;
            }
            return;
        }
    }
}
