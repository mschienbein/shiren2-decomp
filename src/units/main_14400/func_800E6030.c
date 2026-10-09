#include "common.h"

typedef unsigned char u8;

typedef struct Pos800E6030 {
    s32 x;
    s32 y;
} Pos800E6030;

typedef struct Dir {
    u8 value;
} Dir;

/* Partial view: the actor's map position is its first member. */
typedef struct Actor800E6030 {
    Pos800E6030 pos;
} Actor800E6030;

void *func_800A2594(Pos800E6030 *out, void *arg, Dir cell);
void *func_800B4928(Pos800E6030 *pos);
void func_800A59A4(void *self);
void *func_800A7DEC(void *value);
s32 func_800A4CC4(void *p, void *v, void *a);
void func_800A58FC(void *self, Pos800E6030 *pos);
s32 func_800A4EFC(void *self, void *dir);
void func_800A4EC0(void *p, void *a);

/* Swap places with the unit standing in direction `dir`, else move normally. */
s32 func_800E6030(Actor800E6030 *actor, Dir *dir)
{
    Pos800E6030 from_pos;
    Pos800E6030 to_pos;
    Dir back_dir;
    Pos800E6030 *from = &from_pos;
    Pos800E6030 *to = &to_pos;
    Dir *back;
    Actor800E6030 *other;
    s32 ok;
    s32 turn;

    from->x = actor->pos.x;
    from->y = actor->pos.y;
    func_800A2594(to, actor, *dir);
    other = func_800B4928(to);
    back_dir = *dir;
    back = &back_dir;
    turn = back_dir.value + 4;
    back_dir.value = turn & 7;
    if (other != 0) {
        func_800A59A4(actor);
        func_800A59A4(other);
        ok = 0;
        if (func_800A4CC4(actor, func_800A7DEC(actor), dir)) {
            ok = func_800A4CC4(other, func_800A7DEC(other), back) != 0;
        }
        if (ok) {
            func_800A4EC0(actor, dir);
            func_800A4EC0(other, back);
            return 1;
        }
        func_800A58FC(actor, from);
        func_800A58FC(other, to);
    }
    return func_800A4EFC(actor, dir);
}
