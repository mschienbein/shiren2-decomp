#include "common.h"
typedef struct { char pad[0x18]; short delta; short index; void (*fn)(void *, s32, void *); } StreamVT;
typedef struct { char pad[0x18]; StreamVT *vt; } Stream;
typedef struct { char pad[0x20]; short delta; short index; void (*fn)(void *, Stream *); } UnitVT;
typedef struct {
    char pad0[0xA];
    unsigned char fA;
    char padB[0x1E - 0xB];
    unsigned char f1E;
    char pad1F[0x24 - 0x1F];
    UnitVT *vt;
    char pad28[0xE4 - 0x28];
} Unit;
extern char D_80153650[];
extern unsigned char D_801C51A4[];
extern const unsigned char D_8015488C[8];
extern Unit D_801C36EC[];
void func_800CA4A4(Stream *s, void *name);
s32 func_800E0F40(Unit *u);
static inline unsigned char Unit_level(Unit *u) { return (unsigned char)func_800E0F40(u); }
void func_800A8D04(Stream *s) {
    s32 i;
    unsigned char buf[2];
    /* local-arithmetic-qualification: bitmap indexing needs addu v0,v0,s4. */
    u32 mask;
    func_800CA4A4(s, D_80153650);
    s->vt->fn((char *)s + s->vt->delta, 4, D_801C51A4);
    i = 0;
    mask = (u32)D_801C51A4;
    for (;;) {
        Unit *u;
        if (i >= 29) {
            break;
        }
        if (*(unsigned char *)((i >> 3) + mask) & D_8015488C[i & 7]) {
            u = &D_801C36EC[i];
            buf[0] = u->fA;
            buf[1] = 1;
            if (u->f1E & 0x7C) {
                buf[1] = Unit_level(u);
            }
            s->vt->fn((char *)s + s->vt->delta, 1, &buf[0]);
            s->vt->fn((char *)s + s->vt->delta, 1, &buf[1]);
            u->vt->fn((char *)u + u->vt->delta, s);
        }
        i++;
    }
}
