#include "common.h"
typedef unsigned char u8;
typedef signed short s16;
typedef struct { u8 pad[0x28]; s16 delta; s16 idx; void (*fn)(void *, s32, void *); } StreamVTable;
typedef struct { u8 pad[0x18]; StreamVTable *vt; } Stream;
typedef struct { u8 value; u8 id; u8 data[1]; } Obj;
extern u8 D_80153A44[];
void func_800CA4E8(Stream *s, void *tag);
u8 func_800AC1AC(u8 id);
void func_800AF174(Obj *o, Stream *s) {
    func_800CA4E8(s, D_80153A44);
    o->value = func_800AC1AC(o->id);
    s->vt->fn((u8 *)s + s->vt->delta, 1, o->data);
}
