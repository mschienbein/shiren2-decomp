#include "common.h"
typedef struct { char pad[0x38]; short delta; short index; s32 (*fn)(void *, void *); } EntVT;
typedef struct {
    unsigned char kind;
    unsigned char sub;
    char pad2[0x8 - 0x2];
    EntVT *vt;
    unsigned char fC;
} Entity;
typedef struct { s32 f0; s32 f4; } EntIter;
extern unsigned char D_801476BC;
extern unsigned short D_8014767C;
s32 func_80046240(void);
EntIter *func_800B07F0(EntIter *);
s32 func_800B0808(EntIter *it);
Entity *func_800B0864(EntIter *it);
void func_800C94E8(void);
/* ODD_C: the bit test returns a signed-char truth value; its extra pre-combine
   extensions keep the loop large enough that loop.c hoists only the 0x10 and
   0x24 compare constants, leaving li 0x27 inside the loop as in the ROM. */
static inline signed char flagBit(u32 flags, s32 bit) { return (flags >> bit) & 1; }
void func_800C8FAC(void) {
    EntIter it;
    s32 info[8];
    D_801476BC &= ~3;
    if (func_80046240() != 0) {
        return;
    }
    func_800B07F0(&it);
    info[0] = 0x1F;
    for (;;) {
        Entity *e;
        s32 pending;
        s32 matched;
        if (!func_800B0808(&it)) {
            return;
        }
        e = func_800B0864(&it);
        pending = 0;
        if (e->kind == 0x10) {
            pending = flagBit(e->fC, 2);
        }
        if (pending) {
            e->fC &= ~4;
            func_800C94E8();
            continue;
        }
        {
            s32 idle = flagBit(D_8014767C, 6) != 1;
            if (!idle) {
                continue;
            }
        }
        matched = 0;
        if (e->sub == 0x24 || e->sub == 0x27) {
            matched = 1;
        }
        if (!matched) {
            continue;
        }
        e->vt->fn((char *)e + e->vt->delta, info);
    }
}
