#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef struct { s32 x, y; } Vec;
typedef struct { u8 v; } Dir;
typedef struct { unsigned hi : 7; unsigned noBlock : 1; unsigned lo : 24; } Traits;
typedef struct Obj {
    Vec pos;                /* 0x00 */
    Dir dir;                /* 0x08 */
    char pad09[0x15];
    u8 b1E;                 /* 0x1E */
    char pad1F;
    Traits traits;          /* 0x20 */
} Obj;
typedef struct { Obj *obj; s8 state; char pad5[3]; } Entry;
typedef struct {
    Obj *user;              /* 0x00 */
    Vec pos;                /* 0x04 */
    Dir dir;                /* 0x0C */
    char pad0D[3];
    Vec front;              /* 0x10 */
    Traits traits;          /* 0x18 */
    u8 b1C;                 /* 0x1C */
    u8 b1D;                 /* 0x1D */
    u8 b1E;                 /* 0x1E */
    u8 pierce;              /* 0x1F */
    u8 ghost;               /* 0x20 */
    u8 multi;               /* 0x21 */
    u8 step;                /* 0x22 */
    u8 b23;                 /* 0x23 */
    u8 mask;                /* 0x24 */
    u8 hitAny;              /* 0x25 */
    u8 count;               /* 0x26 */
    char pad27;
    /* func_801106BC supplies this complete 0xA8-byte record. */
    Entry entries[16];       /* 0x28 */
} Plan;
extern s8 D_8015D3EC[];
extern s8 D_8015D3F4[];
extern void *func_800A2594(Vec *, Vec *, Dir);
extern Vec *func_800A25D8(Vec *, Vec *, Dir, s32);
extern s32 func_8010BEC4(void *, u8);
extern s32 func_8010EBA4(void *);
extern s32 func_800E1CC4(Obj *, s32);
extern u32 func_800B1C6C(Vec *);
extern s32 func_8010FC28(void *, Obj *, Plan *, Vec *);
extern Obj *func_8010FD64(void *, Plan *, Vec *, s32);
extern s32 func_800E547C(Obj *, Obj *);
extern void *func_800A7DEC(Obj *);
extern s32 func_800A4CC4(Obj *, void *, Dir *);
extern s32 func_800E1CD4(Obj *, s32);
/* Trait snapshot through a stack copy, the same idiom as is_locked in func_8010FD64. */
static inline s32 is_ghost(Traits *copy, Obj *user) {
    *copy = user->traits;
    return copy->noBlock;
}
/* A wall at POS, or an obstacle met when stepping back from it into BACK, stops the shot. */
static inline s32 path_blocked(void *self, Obj *user, Plan *plan, Vec *pos, Vec *back) {
    s32 stop = 0;
    if (func_800B1C6C(pos) & 0x4000) {
        stop = 1;
    } else {
        Dir d;
        d.v = (plan->dir.v + 4) & 7;
        func_800A2594(back, pos, d);
        if (func_8010FC28(self, user, plan, back)) stop = 1;
    }
    return stop;
}

/* Member-wise position copy. */
static inline void copy_pos(Vec *to, Vec *from) {
    to->x = from->x;
    to->y = from->y;
}

void func_8010F020(void *self, Plan *plan, Obj *user) {
    Vec v;
    Vec cur;
    Vec next;
    Traits traits;
    Dir back;
    s8 none;
    s8 *list;
    Entry *e;
    s32 range;
    s32 blocked;
    u32 attr;
    s32 flag;
    u8 clear;
    Vec *side;
    plan->user = user;
    plan->pos = user->pos;
    plan->dir = user->dir;
    func_800A2594(&cur, &plan->pos, plan->dir);
    plan->front = cur;
    plan->traits = user->traits;
    if ((u8)func_8010BEC4(self, 0x52)) plan->b1C = 0x52;
    else if ((u8)func_8010BEC4(self, 0x37)) plan->b1C = 0x37;
    else plan->b1C = 0;
    if ((u8)func_8010BEC4(self, 0x58)) plan->b1D = 0x58;
    else if ((u8)func_8010BEC4(self, 0x49)) plan->b1D = 0x49;
    else plan->b1D = 0;
    plan->b1E = func_8010EBA4(self);
    flag = 0;
    clear = func_800E1CC4(user, 5) ^ 1; /* not under status 5 */
    if (clear && (plan->b1E || (u8)func_8010BEC4(self, 0x38))) flag = 1;
    plan->pierce = flag;
    flag = 0;
    if (is_ghost(&traits, user) || (u8)func_8010BEC4(self, 0x78)) flag = 1;
    range = 1;
    plan->ghost = flag;
    plan->count = 0;
    plan->mask = 0;
    plan->multi = 0;
    if ((u8)func_8010BEC4(self, 0x4C)) {
        range = 3;
    } else if ((u8)func_8010BEC4(self, 0x4B)) {
        range = 2;
        plan->multi = 1;
    }
    attr = func_800B1C6C(&plan->front) & 0x4000;
    blocked = attr != 0;
    plan->hitAny = 0;
    if (func_8010FC28(self, user, plan, &plan->pos)) {
        plan->step = 1;
    } else {
        plan->step = 0;
        side = &next;
        for (;;) {
            Obj *hit;
            s32 out;
            s32 struck;
            if (plan->step >= range) break;
            plan->step++;
            func_800A25D8(&cur, &plan->pos, plan->dir, plan->step);
            out = 0;
            if (cur.y >= 0x4C || cur.x >= 0x36 || cur.y < 0 || cur.x < 0) out = 1;
            if (out) break;
            if (plan->ghost == 0 && path_blocked(self, user, plan, &cur, side)) {
                blocked = 1;
                break;
            }
            copy_pos(&next, &cur);
            hit = func_8010FD64(self, plan, &next, plan->step == 1);
            if (hit == 0) continue;
            if (plan->step == 1) plan->mask |= 1;
            plan->hitAny = 1;
            e = &plan->entries[plan->count++];
            e->obj = hit;
            e->state = -1;
            struck = 0;
            if (plan->pierce || (hit->b1E & 3) || func_800E547C(hit, user)) struck = 1;
            if (struck) {
                e->state = 1;
                blocked = 1;
                if (!plan->multi) break;
            } else {
                e->state = 0;
            }
        }
    }
    plan->b23 = 0;
    if ((u8)func_8010BEC4(self, 0x44) && blocked) {
        Obj *u;
        u8 retreat = 0;
        back.v = (plan->dir.v + 4) & 7;
        u = plan->user;
        if ((u8)func_800A4CC4(u, func_800A7DEC(u), &back)) {
            u8 free = func_800E1CD4(plan->user, 0x10) ^ 1; /* not under status 0x10 */
            retreat = free != 0;
        }
        plan->b23 = retreat;
    }
    none = 0;
    list = &none;
    if ((u8)func_8010BEC4(self, 0x4D)) list = D_8015D3EC;
    else if ((u8)func_8010BEC4(self, 0x3A)) list = D_8015D3F4;
    e = &plan->entries[plan->count];
    for (;;) {
        s32 off;
        Obj *hit;
        Dir d;
        off = *list;
        list++;
        if (off == 0) break;
        d.v = (plan->dir.v + off) & 7;
        func_800A2594(&cur, &plan->pos, d);
        /* ODD_C: value-preserving round trip through v (block copy out, member-wise copy
           back). The ROM performs both copies; dropping them is 46 words / 1340 bytes and
           copying the members directly is 49 words / 1372 bytes. */
        v = cur;
        copy_pos(&cur, &v);
        hit = func_8010FD64(self, plan, &cur, 0);
        if (hit == 0) continue;
        e->obj = hit;
        e->state = -1;
        e++;
        plan->count++;
        plan->mask |= 1 << off;
    }
}
