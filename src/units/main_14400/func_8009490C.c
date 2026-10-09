#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef struct { u8 kind; u8 data[0x22]; } Rec;
/* Stream writer slot +0x1C: void (self, byte count, source buffer). */
typedef struct { short delta; short index; void (*fn)(void *, s32, void *); } VEntry;
typedef struct { char pad[0x18]; VEntry *vtbl; } Sink;
typedef struct { short delta; short index; s32 (*fn)(void *self, u8 *buffer); } SrcEntry;
typedef struct { s32 pad0; SrcEntry *vtbl; } Source;
typedef struct {
    Sink *sink;
    u8 pad4[4];
    Rec cur;
    Rec next;
    u8 pad4E[2];
    u8 unk50[8];
    u8 unk58[8];
    u8 step;
} Obj;
void func_800CA408(Sink *, void *);
void func_800CA3F4(Sink *, void *);
s32 func_80083D8C(void *left, void *right, u32 count);
static inline void Sink_send(Sink *k, s32 n, void *p) {
    VEntry *e = &k->vtbl[3];
    e->fn((char *)k + e->delta, n, p);
}
static inline void Sink_repeat(Sink *k, s32 n, u8 step) {
    VEntry *e = &k->vtbl[3];
    u8 message = step | 0x80;
    e->fn((char *)k + e->delta, n, &message);
}
void func_8009490C(Obj *self, Source *src) {
    SrcEntry *se = &src->vtbl[3];
    s32 n = se->fn((char *)src + se->delta, (u8 *)&self->next);
    if (n <= 0) return;
    if (self->cur.kind != 0) {
        if (self->cur.kind == 3 && (self->next.kind == 2 || self->next.kind == 3)) {
            func_800CA408(self->sink, self->unk50);
        } else if (func_80083D8C(&self->next, &self->cur, n) == 0 && (s8)self->step >= 0) {
            s32 cnt;
            u8 s;
            Sink *k;
            if (self->step == 0) {
                func_800CA3F4(self->sink, self->unk58);
                cnt = 1;
            } else {
                func_800CA408(self->sink, self->unk58);
                cnt = 1;
            }
            s = self->step;
            k = self->sink;
            self->step = s + cnt;
            Sink_repeat(k, cnt, s);
            return;
        }
    }
    self->step = 0;
    func_800CA3F4(self->sink, self->unk50);
    Sink_send(self->sink, n, &self->next);
    self->cur = self->next;
}
