#include "common.h"
typedef unsigned char u8;
typedef struct { s32 unk0; s32 unk4; } Msg;
/* Slots +0x24/+0x2C take only self; +0x84 also takes a cursor position. */
typedef struct {
    short delta;
    short index;
    union {
        void (*self)(void *);
        void (*position)(void *, Msg *);
    } fn;
} VEntry;
typedef struct {
    char pad[0x4C];
    VEntry *vtbl;
    char pad50[0x70 - 0x50];
    u8 buf[8];
    s32 cursor;
    s32 len;
} Obj;
typedef struct { s32 a; s32 b; } Sel;
extern s32 D_801528B0;
void func_80045A24(s32);
void func_8009AF4C(Obj *, s32);
void func_80048764(Obj *);
s32 func_800B0E80(s32);
u8 *func_800B0A70(u8, s32);
u8 *func_8006A810(void *, s32, s32);
s32 func_8009BB74(Obj *self, Sel *sel) {
    VEntry *e;
    func_80045A24(0);
    if (sel->a == 0) {
        if (sel->b != 0) return 0;
        func_8009AF4C(self, 0);
        func_80048764(self);
        e = &self->vtbl[4];
        e->fn.self((char *)self + e->delta);
    } else {
        u8 *s = func_800B0A70(func_800B0E80(sel->b * 6 + sel->a - 1), 1);
        if (s) {
            s32 i;
            Msg m;
            for (i = 0; i < self->len; i++) {
                self->buf[i] = *s;
                if (*s++ == 0) break;
            }
            if (i < self->len) {
                self->cursor = i;
            } else {
                self->cursor = self->len - 1;
            }
            for (; i < self->len; i++) {
                self->buf[i] = 0;
            }
            func_8006A810(&m, 0, 8);
            m.unk4 = D_801528B0;
            e = &self->vtbl[16];
            e->fn.position((char *)self + e->delta, &m);
        }
    }
    e = &self->vtbl[5];
    e->fn.self((char *)self + e->delta);
    return 1;
}
