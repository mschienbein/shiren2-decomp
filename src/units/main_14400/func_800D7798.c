#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
/* Entity record view: kind at +1, flags at +0xC, map coordinates at +0xD/+0xE. */
typedef struct { u8 pad0; u8 kind; u8 pad2[0xA]; u8 flags; u8 x; u8 y; } Ent;
/* List iterator used by func_800B07F0/func_800B0808/func_800B0864: list
   pointer at +0, 16-bit cursor at +4 (eight-byte object). */
typedef struct { void *list; u16 cursor; } Iter;
extern u8 D_801480DC[21];          /* bitmap of marked cells */
extern const u8 D_8015488C[8];     /* single-bit masks */
extern Iter *func_800B07F0(Iter *it);
extern s32 func_800B0808(Iter *it);
extern Ent *func_800B0864(Iter *it);
extern s32 func_800D7D84(u8 x, u8 y);

/* ODD_C: flag test as an accessor; the inlined `!= 0` expands through the sne
   template with a hard-coded $0, which reload_cse cannot rewrite to the
   known-zero `ok` register (same mechanism as func_80100A10's nonzero). */
static inline s32 has_flag(Ent *e, u32 mask) { return (e->flags & mask) != 0; }

void func_800D7798(void) {
    Iter it;
    u8 *p = D_801480DC;
    s32 n = 20;
    do {
        *p++ = 0;
    } while (n-- > 0);
    func_800B07F0(&it);
    while (func_800B0808(&it)) {
        Ent *e;
        s32 ok;
        s32 bit;
        u8 *bits;
        e = func_800B0864(&it);
        ok = e->kind == 0xF1 && has_flag(e, 4);
        if (ok) {
            bit = func_800D7D84(e->x, e->y);
            bits = &D_801480DC[bit >> 3];
            *bits |= D_8015488C[bit & 7];
        }
    }
}
