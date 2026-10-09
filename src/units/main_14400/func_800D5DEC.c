#include "common.h"

typedef unsigned char u8;

typedef struct { s32 x; s32 y; } Pos;
typedef struct { u8 value; } Dir;

typedef struct {
    u32 pad0 : 8;
    u32 enabled : 1;
    u32 pad9 : 23;
} WordFlags;

typedef struct {
    u8 pad0[0x20];
    WordFlags flags;
} Unit;

typedef struct {
    u8 kind;
} Thing;

extern Unit *D_80147FE0;
extern Pos D_80148000;   /* path start position */
extern Pos D_80148024;  /* path goal */
extern u8 D_80148038;   /* path length */
extern u8 D_8014803C[]; /* path directions */
extern u8 D_80148085;

s32 func_800A251C(Pos *x, Pos *y);
void *func_800A2594(Pos *out, void *arg, Dir cell);
void *func_800A27A4(void *out_direction, void *from, void *to);
u8 func_800D60D4(Pos *from, Pos *to, Dir dir, u8 *out);
s32 func_800D71F0(Pos *pos, Dir *dir);
u32 func_800B1C6C(Pos *pos);
void *func_800B4D80(Pos *p);
int func_800A6E50(Unit *arg0, Thing *arg1);
void *func_800A7DEC(void *value);
s32 func_800A4CC4(void *p, void *v, void *a);

static inline Pos *copyPos(Pos *out, Pos *in) { out->x = in->x; out->y = in->y; return out; }
u8 func_800D5DEC(void) {
    u8 dirs[8];
    struct {
        Pos cur;
        Pos prev;
        Pos next;
        Pos goal;
    } w;
    Dir dir;
    Dir step_dir;
    WordFlags flags;
    Dir *sd;
    s32 step;

    copyPos(&w.cur, &D_80148000);
    copyPos(&w.prev, &D_80148000);
    for (step = 0; ; step++) {
        u8 count;
        s32 k;

        if (step >= 0x40) {
            break;
        }
        if (func_800A251C(&w.cur, &D_80148024)) {
            break;
        }
        if (step > 0 && func_800A251C(&w.cur, &D_80148000)) {
            break;
        }
        w.next.x = D_80148024.x;
        w.next.y = D_80148024.y;
        func_800A27A4(&dir, &w.cur, &w.next);
        k = 0;
        sd = &step_dir;
        copyPos(&w.next, &w.cur);
        copyPos(&w.goal, &D_80148024);
        count = func_800D60D4(&w.next, &w.goal, dir, dirs);
        for (; ; k++) {
            s32 blocked;
            Dir *d;
            s32 at_goal;

            if (k >= count) {
                break;
            }
            step_dir.value = dirs[k] & 7;
            func_800A2594(&w.next, &w.cur, step_dir);
            blocked = 0;
            d = &step_dir;
            if (func_800A251C(&w.prev, &w.next)) {
                blocked = 1;
            } else {
                at_goal = func_800A251C(&w.next, &D_80148024) == 1;
                if ((!at_goal && !(func_800B1C6C(&w.next) & 0x1000)) || !func_800D71F0(&w.cur, d)) {
                    blocked = 1;
                }
            }
            if (blocked) {
                continue;
            }
            if (step == 0 && dirs[k] != D_80148085) {
                Thing *thing = func_800B4D80(&w.next);

                if (thing != 0 && thing->kind == 0x10) {
                    s32 bad = 0;

                    if (func_800A6E50(D_80147FE0, thing)) {
                        Unit *unit = D_80147FE0;
                        WordFlags *saved = &flags;
                        u32 enabled = unit->flags.enabled;

                        *saved = unit->flags;
                        bad = !enabled;
                    }
                    if (bad) {
                D_80148038 = 0;
                return 0;
            }
                }
                {
                    Unit *unit = D_80147FE0;
                    s32 allowed = func_800A4CC4(unit, func_800A7DEC(unit), sd) == 1;

                    if (!allowed) {
                D_80148038 = 0;
                return 0;
            }
                }
            }
            D_8014803C[step] = dirs[k];
            w.prev = w.cur;
            w.cur = w.next;
            break;
        }
        if (k >= count) {
            D_80148038 = 0;
            return 0;
        }
    }
    D_80148038 = step;
    return step;
}
