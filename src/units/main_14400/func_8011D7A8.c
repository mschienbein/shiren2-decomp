#include "common.h"
typedef unsigned short u16;
typedef unsigned char u8;
typedef struct { s32 x; s32 y; } Pos;
typedef struct { u8 v; } Dir;
typedef struct { short delta; short index; void (*fn)(void *, s32); } VtEntry;
typedef struct { char pad[8]; VtEntry *vt; } Actor;
typedef struct { char pad[9]; u8 unk9; } Unit;
typedef struct { s32 kind; void *source; Unit *unit; u8 dir; char padD[0xB]; s32 unk18; void *origin; } Msg;
s32 func_80049CB4(s32, ...);
Pos *func_800A25D8(Pos *, Pos *, Dir, s32);
void *func_800A2594(Pos *, void *, Dir);
s32 func_800B1AB8(Pos *);
s32 func_800B1DF8(Pos *);
Unit *func_800B4928(Pos *);
s32 func_800A58B8(Unit *);
s32 func_800B58E4(s32, s32, s32, s32);
char *func_800AC990(void *);
s32 func_800A5440(Unit *, void *, u8 *, s32, char *);
void func_80112864(Actor *, Msg *);
void func_800A2758(Pos *, Dir);
void func_800A2F80(Dir *, s32);
void func_800D3650(Actor *);
/* ODD_C: the message setters and loop predicate keep the message, direction and iteration
 * temporaries distinct (the direction setter's Dir pointer is what GCC hoists for the push call). */
static inline void Msg_setUnk18(Msg *m, s32 v){ m->unk18 = v; }
static inline void Msg_setDir(Msg *m, Dir *d){ m->dir = d->v; }
static inline s32 Below(s32 v, s32 n){ return v < n; }
void func_8011D7A8(Actor *self, void *source, Pos *const pos, Dir dir, u16 flags) {
    Pos target;
    Pos cur;
    Msg msg;
    s32 i, ok;
    Unit *u;

    func_80049CB4(0x131);
    i = 0;
    dir.v = (dir.v - 1) & 7;
    for (; Below(i, 3); i++) {
        func_800A25D8(&target, pos, dir, 10);
        func_80049CB4(6);
        func_80049CB4(0xB9, self, pos, &target);
        func_800A2594(&cur, pos, dir);
        for (;;) {
            ok = 0;
            if (func_800B1AB8(&cur)) ok = func_800B1DF8(&cur) == 0;
            if (!ok) break;
            u = func_800B4928(&cur);
            if (u) {
                s32 kind = u->unk9 & 0xF;
                if (func_800B58E4(2, 2, kind, func_800A58B8(u))) {
                    if (!func_800A5440(u, source, &dir.v, flags | 0x10, func_800AC990(self))) {
                        msg.kind = 0x13;
                        msg.source = source;
                        msg.unit = u;
                        Msg_setDir(&msg, &dir);
                        Msg_setUnk18(&msg, 1);
                        msg.origin = source;
                        func_80112864(self, &msg);
                    }
                }
            }
            func_800A2758(&cur, dir);
        }
        func_800A2F80(&dir, 1);
        func_80049CB4(7);
    }
    func_80049CB4(0x132);
    func_800D3650(self);
    if (self) self->vt[1].fn((char *)self + self->vt[1].delta, 3);
}
