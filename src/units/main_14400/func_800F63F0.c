#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef struct { s32 x, y; } Pos;
typedef u8 Direction;
typedef struct {
    Pos pos;
    u8 pad8[0x50];
    void *x58;
    u8 pad5C[0x24];
    void *x80;
    Pos x84;
    Pos x8C;
    s32 x94;
    s32 x98;
    u8 x9C;
    u8 pad9D[3];
    s32 xA0;
    s32 xA4;
} S;
s32 func_800E8694(S *);
s32 func_800E8350(S *);
s32 func_800E7104(S *);
void *func_800A492C(S *, s32, s32, s32);
s32 func_800A251C(Pos *, Pos *);
void func_800F6038(S *, s32);
s32 func_800D2FB0(void *);
void *func_800B1F90(S *);
s32 func_800A31C8(void *, Pos *);
s32 func_800A23E8(Pos *, Pos *);
s32 func_800A4314(S *, Pos *);
s32 func_800E65C0(S *, Pos *, s32);
s32 func_800E66EC(S *);
/* The first argument is the original one-byte hidden result buffer. */
Direction *func_800A6538(Direction *, S *, Pos *);
void func_800A665C(S *, Direction *);
s32 func_800F63F0(S *s) {
    Pos p;
    Pos q;
    Pos r;
    Direction dir;
    Pos *t;
    void *obj;
    s32 hit1;
    s32 hit2;
    Pos *pp;
    s32 done;
    s32 ok;

    if (func_800E8694(s)) {
        return func_800E8350(s);
    }
    if (s->x94) {
        s->x58 = func_800A492C(s, 2, 1, 1);
        return func_800E7104(s);
    }
    if (s->x98) {
        if (s->x9C == 0) {
            s->x98 = 0;
            s->x58 = 0;
        } else if (s->x58 != 0) {
            s->x9C--;
            return func_800E7104(s);
        } else {
            s->x98 = 0;
            s->x9C = 0;
        }
    }
    pp = &p;
    pp->x = s->pos.x;
    pp->y = s->pos.y;
    hit1 = func_800A251C(pp, &s->x84);
    hit2 = func_800A251C(pp, &s->x8C);
    if (!hit1 && !hit2) {
        func_800F6038(s, 2);
    }
    t = func_800D2FB0(s->x80) ? &s->x8C : &s->x84;
    q.x = t->x;
    q.y = t->y;
    if (func_800A251C(&p, &q)) {
        s->xA0 = 0;
        s->xA4 = 0;
        return 0;
    }
    obj = func_800B1F90(s);
    if (obj != 0 && func_800A31C8(obj, &q)) {
        done = 0;
        r.x = q.x;
        r.y = q.y;
        if (func_800A23E8(&p, &r) == 1) {
            done = func_800A4314(s, &q) == 0;
        }
        if (done) return 0;
        ok = func_800E65C0(s, &q, 0);
    } else {
        ok = func_800E66EC(s);
    }
    if (!ok) return 0;
    p = s->pos;
    if (func_800A251C(&p, &q)) {
        func_800F6038(s, 1);
        if (s->xA0) s->xA0 = 0;
        if (s->xA4) s->xA4 = 0;
        if (func_800A251C(&p, &s->x84)) {
            func_800A6538(&dir, s, &s->x8C);
            func_800A665C(s, &dir);
        }
    }
    return 1;
}
