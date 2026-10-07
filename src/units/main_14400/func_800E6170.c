#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { s32 x; s32 y; } Pos;
typedef struct { u8 value; } Dir;
typedef struct { Pos pos; Dir dir; } Mover;
extern u8 D_80147620[];
extern s8 D_80148338[];
u8 func_800C57A0(void *rng);
void func_800A2F80(Dir *d, s32 n);
void func_800A2F94(Dir *d, s32 n);
s32 func_800A4CC4(Mover *m, Pos *p, Dir *d);
void *func_800A2594(void *out, void *from, Dir dir);
s32 func_800A31C8(void *map, Pos *p);
void func_800A4EC0(Mover *m, Dir *d);
s32 func_800E6170(Mover *m, void *map) {
    Pos p;
    Pos next;
    Dir d;
    s32 found = 0;
    s32 i;
    u8 r;
    p.x = m->pos.x;
    p.y = m->pos.y;
    d = m->dir;
    r = func_800C57A0(D_80147620);
    if (r < 20) {
        func_800A2F80(&d, 1);
    } else if (r < 40) {
        func_800A2F94(&d, 1);
    }
    for (i = 7; i != -1; i--) {
        s32 ok;
        func_800A2F80(&d, D_80148338[i]);
        ok = 0;
        if (func_800A4CC4(m, &p, &d)) {
            func_800A2594(&next, &p, d);
            ok = func_800A31C8(map, &next) != 0;
        }
        if (ok) {
            found = 1;
            break;
        }
    }
    if (found) {
        func_800A4EC0(m, &d);
        return 1;
    }
    return 0;
}
