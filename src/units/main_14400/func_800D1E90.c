#include "common.h"
typedef unsigned char u8;
typedef struct { s32 x0, y0, x1, y1; } Rect;
typedef struct { s32 x; s32 y; } Pos;
typedef struct { u8 value; } Dir;
typedef struct { s32 index; } Iter;
typedef struct { char pad0[0x84]; Pos pos84; Pos pos8C; } Actor;
typedef struct { s32 unk0; Rect *bounds_04; } Ctx;
extern u8 D_80147620[]; /* random number generator object */
void *func_800B3774(Pos *out);
s32 func_800A31C8(Rect *rect, Pos *pos);
unsigned char func_800C57A0(void *rng);
void *func_800A2594(Pos *out, Pos *from, Dir dir);
s32 func_800A41EC(Actor *actor, Pos *pos);
s32 func_800B1E80(Pos *pos);
u32 func_800B1C6C(Pos *pos);
s32 func_800A9070(Iter *it, s32 kind);
Actor *func_800A910C(Iter *it);
s32 func_800A251C(Pos *a, Pos *b);

static inline void Dir_set(Dir *dir, u8 value) {
    dir->value = value;
}

static inline u8 Random_dir(void) {
    return func_800C57A0(D_80147620) & 7;
}

static inline void Pos_copy(Pos *dst, Pos *src) {
    dst->x = src->x;
    dst->y = src->y;
}

/* *dst = the cell one step from *from in direction dir (func_800A2594 fills a temporary). */
static inline void Pos_step(Pos *dst, Pos *from, Dir dir) {
    Pos t;
    func_800A2594(&t, from, dir);
    *dst = t;
}

/* Starting cell: the origin, or a random cell from func_800B3774. */
static inline void Pos_start(Pos *dst, s32 useOrigin) {
    Pos start;
    if (useOrigin) {
        start.x = 0;
        start.y = 0;
    } else {
        func_800B3774(&start);
    }
    *dst = start;
}

/* Whether another kind-0x57 actor's pos84 is at *at. */
static inline u8 Actor_other_at(Actor *self, Pos *at) {
    s32 found = 0;
    Iter it;
    Actor *other;
    it.index = 0;
    while (func_800A9070(&it, 0x57)) {
        other = func_800A910C(&it);
        if (other == self) {
            continue;
        }
        {
            Pos tmp;
            Pos_copy(&tmp, &other->pos84);
            if (func_800A251C(at, &tmp)) {
                found = 1;
                break;
            }
        }
    }
    return found;
}

s32 func_800D1E90(Ctx *ctx, Actor *self, Pos *origin, Pos *out, Pos *pos, s32 useOrigin) {
    s32 tries;
    s32 found;
    s32 ok;
    s32 i;
    s32 base;
    s32 failed;

    Pos_start(pos, useOrigin);
    if (func_800A31C8(ctx->bounds_04, pos) != 0) {
        /* Random walk from *pos: up to 99 tries for a free cell next to it. */
        tries = 100;
        while (1) {
            if (--tries == -1) {
                return 0;
            }
            {
                Dir dir;
                Dir_set(&dir, Random_dir());
                Pos_step(out, pos, dir);
            }
            failed = func_800A41EC(self, out) ^ 1;
            if (failed) {
                continue;
            }
            if (func_800B1E80(out) != 0) {
                continue;
            }
            if (tries >= 50 && !(func_800B1C6C(out) & 0x1000)) {
                continue;
            }
            found = Actor_other_at(self, out);
            if (!found) {
                break;
            }
        }
        return 1;
    }
    {
        u8 dirs[7] = { 3, 1, 5, 7, 3, 1, 5 };

        /* Four diagonal neighbours of *origin, from a random starting one. */
        ok = 0;
        base = func_800C57A0(D_80147620) & 3;
        for (i = ok; ; i++) {
            Dir dir;
            if (i >= 4) {
                break;
            }
            Dir_set(&dir, dirs[base + i] & 7);
            Pos_step(out, origin, dir);
            failed = func_800A31C8(ctx->bounds_04, out) ^ 1;
            if (failed) {
                continue;
            }
            if (func_800B1E80(out) != 0) {
                continue;
            }
            found = 0;
            {
                Iter it;
                Actor *other;
                it.index = 0;
                while (func_800A9070(&it, 0x57)) {
                    other = func_800A910C(&it);
                    if (other == self) {
                        continue;
                    }
                    {
                        Pos tmp;
                        tmp.x = other->pos84.x;
                        tmp.y = other->pos84.y;
                        if (func_800A251C(out, &tmp)) {
                            found = 1;
                            break;
                        }
                    }
                }
            }
            if (!found) {
                ok = 1;
                break;
            }
        }
        if (!ok) {
            return 0;
        }

        /* Even directions around *origin for *pos, checked against pos8C. */
        ok = 0;
        for (i = ok; ; i += 2) {
            Dir dir;
            if (i >= 7) {
                break;
            }
            Dir_set(&dir, i & 7);
            Pos_step(pos, origin, dir);
            failed = func_800A31C8(ctx->bounds_04, pos) ^ 1;
            if (failed) {
                continue;
            }
            failed = func_800B1E80(pos) ^ 1;
            if (failed) {
                continue;
            }
            found = 0;
            {
                Iter it;
                Actor *other;
                it.index = 0;
                while (func_800A9070(&it, 0x57)) {
                    other = func_800A910C(&it);
                    if (other == self) {
                        continue;
                    }
                    {
                        Pos tmp;
                        tmp.x = other->pos8C.x;
                        tmp.y = other->pos8C.y;
                        if (func_800A251C(pos, &tmp)) {
                            found = 1;
                            break;
                        }
                    }
                }
            }
            if (!found) {
                ok = 1;
                break;
            }
        }
    }
    return ok;
}
