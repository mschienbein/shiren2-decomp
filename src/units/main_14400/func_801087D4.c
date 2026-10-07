#include "common.h"
typedef unsigned char u8;
typedef struct { u8 value; } Dir;
typedef struct { s32 x, y; } Pair;
typedef struct {
    s32 x0;
    s32 x4;
    char pad8[0x50];
    Pair *target;
    char pad5C[0x2D];
    unsigned char x89;
    unsigned char x8A;
    char pad8B[0x15];
    unsigned char xA0;
    char padA1[3];
    s32 xA4;
} Unit;
static inline s32 get_state(Unit *u) {
    return u->xA4;
}
static inline s32 in_state(Unit *u, s32 s) {
    return get_state(u) == s;
}
static inline void pair_copy(Pair *d, Pair *s) {
    d->x = s->x;
    d->y = s->y;
}
unsigned char func_800A6420(Unit *u, Pair *target);
s32 func_800E7104(Unit *u);
s32 func_800A8AA0(s32 id, s32 arg);
void func_80108660(void *self, s32 kind, u8 filter, s32 value);
s32 func_80049CB4(s32 id, ...);
void func_800498E4(s32 message_id, ...);
void func_80049BF0(s32 v);
extern Dir *func_800A22B8(Dir *, void *, void *);
void func_800A665C(Unit *u, Dir *dir);
s32 func_800A282C(Pair *from, Pair *to);
s32 func_800A4FBC(Unit *u, unsigned char *v);
s32 func_801087D4(Unit *u) {
    Pair *target = u->target;
    unsigned short msg;
    s32 state;
    s32 cond;
    s32 alt;
    cond = in_state(u, 5) || !func_800A6420(u, target);
    if (cond) return func_800E7104(u);
    if (u->xA0 == 0) {
        alt = in_state(u, 1) || in_state(u, 3);
        if (alt) u->xA0 = u->x8A;
        else u->xA0 = u->x89;
        msg = 0;
        state = 5;
        switch (get_state(u)) {
        case 1:
            if (func_800A8AA0(0x50, 1) + func_800A8AA0(0x50, 2)) {
                msg = 0x152;
                state = 2;
                func_80108660(u, 0x50, 1, 3);
                func_80108660(u, 0x50, state, 3);
                break;
            }
        case 2:
            if (func_800A8AA0(0x51, 0)) {
                msg = 0x153;
                state = 3;
                func_80108660(u, 0x51, 1, 2);
                break;
            }
        case 3:
        retry:
            if (func_800A8AA0(0x51, 0)) {
                msg = 0x154;
                state = 4;
                func_80108660(u, 0x51, 0, 3);
                break;
            }
        default:
            if (func_800A8AA0(0x50, 1) + func_800A8AA0(0x50, 2)) {
                msg = 0x151;
                state = 1;
                func_80108660(u, 0x50, 1, 2);
                func_80108660(u, 0x50, 2, 2);
                break;
            }
            if (func_800A8AA0(0x51, 0)) goto retry;
            break;
        }
        u->xA4 = state;
        if (msg != 0) {
            func_80049CB4(0x9B, u);
            func_800498E4(msg);
            func_80049BF0(0);
        }
        return 0;
    } else {
Pair from, to, dst;
        Dir dir;
        u->xA0--;
        pair_copy(&from, (Pair *)u);
        to.x = target->x;
        to.y = target->y;
        if (target != 0) {
            func_800A22B8(&dir, &from, &to);
            func_800A665C(u, &dir);
            dst.x = to.x;
            dst.y = to.y;
            if (func_800A282C(&from, &dst) == 0) return 0;
            if (from.y >= 0x3B) {
                unsigned char v = 4;
                return func_800A4FBC(u, &v);
            } else {
                unsigned char v = 0;
                return func_800A4FBC(u, &v);
            }
        }
    }
    return 0;
}
