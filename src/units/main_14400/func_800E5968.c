#include "common.h"
typedef unsigned char u8;
typedef struct { short delta; short index; void *fn; } VEntry;
typedef struct { u32 pad0 : 15; u32 flag : 1; u32 pad16 : 16; } Bits;
typedef struct { unsigned char unk0; unsigned char unk1; char pad2[6]; VEntry *vtbl; } Msg;
typedef struct { char pad[0xA]; unsigned char unkA; char padB[0x1E - 0xB]; unsigned char unk1E; char pad1F[1]; Bits unk20; } Obj;
typedef struct { s32 type; Obj *obj; char pad8[0x10]; s32 flags; char pad1C[4]; } Event;
typedef struct { unsigned char value; } Pick;
extern char D_80147620[];
u8 func_800C57CC(void *rng, u8 limit);
void func_800A665C(Obj *, Pick *);
s32 func_80049CB4(s32 id, ...);
static inline s32 testFlag(Bits *b) { return b->flag; }
static inline void initEvent(Event *e, s32 type, Obj *o, s32 flags) { e->type = type; e->obj = o; e->flags = flags; }
void func_800E5968(Obj *o, Msg *m, unsigned short flags) {
    Event ev;
    Bits bits;
    Pick pick;
    s32 id;
    bits = o->unk20;
    if (testFlag(&bits)) {
        pick.value = func_800C57CC(D_80147620, 7) & 7;
        func_800A665C(o, &pick);
    }
    if (m->unk0 == 5) {
        func_80049CB4(0x1037, o, m);
    } else {
        if (m->unk1 != 0xCA) id = 0x1029;
        else if ((o->unk1E >> 2) & 1) id = 0x1039;
        else if (o->unkA == 0x37) id = 0x106A;
        else id = 0x1029;
        func_80049CB4(id, o);
    }
    if (m->unk1 == 0xAC) flags |= 4;
    initEvent(&ev, 9, o, flags);
    {
        VEntry *e = &m->vtbl[7];
        ((void (*)(char *, Event *))e->fn)((char *)m + e->delta, &ev);
    }
}
