#include "common.h"
typedef unsigned char u8;
typedef signed short s16;
typedef struct { u8 pad[0x28]; s16 delta; s16 idx; void (*fn)(void *, s32, void *); } StreamVTable;
typedef struct { u8 pad[0x18]; StreamVTable *vt; } Stream;
typedef struct { u8 unk0[0xC]; u8 data[4]; } Obj;
extern u8 D_8015D2F4[];
void func_800AF174(Obj *o, Stream *s);
void func_800CA4E8(Stream *s, void *tag);
void func_8010E1DC(Obj *o, Stream *s) {
    func_800AF174(o, s);
    func_800CA4E8(s, D_8015D2F4);
    s->vt->fn((u8 *)s + s->vt->delta, 4, o->data);
}
