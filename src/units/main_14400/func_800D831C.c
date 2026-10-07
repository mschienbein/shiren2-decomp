#include "common.h"
typedef struct { void *f0; s32 f4; u32 f8; s32 fC; } BitStream;
void *func_800A09B0(void *stream, void *buf, u32 size);
void func_800A09C4(void *stream, void *buf, u32 size);
void func_800A09E8(void *stream, u32 bits, u32 count);
static inline void BitStream_rewind(BitStream *bs) {
    func_800A09C4(bs, bs->f0, bs->f8);
}
typedef struct { char pad[0x18]; short delta; short index; void (*fn)(void *, s32, void *); } VT;
typedef struct { char pad[0x18]; VT *vt; } Obj;
extern char D_80148100[];
extern char D_801480F4[];
extern unsigned char D_80148190[];
extern char D_80154880[];
void func_800CA4A4(Obj *o, void *name);
void func_800D831C(Obj *o) {
    BitStream bs;
    unsigned char *src;
    s32 i;
    func_800A09B0(&bs, D_80148100, 0x8E);
    BitStream_rewind(&bs);
    src = D_80148190;
    i = 0;
    do {
        func_800A09E8(&bs, *src++, 7);
        i++;
    } while (i < 162);
    func_800CA4A4(o, D_80154880);
    o->vt->fn((char *)o + o->vt->delta, 0x8E, D_80148100);
    o->vt->fn((char *)o + o->vt->delta, 9, D_801480F4);
}
