#include "common.h"

typedef unsigned char u8;
typedef signed char s8;

typedef struct { s32 x; s32 y; } Pos800E7104;
typedef struct { u8 value; } Dir;
typedef struct { u32 pad : 5; u32 wary : 1; u32 rest : 26; } Flags800E7104;
typedef struct {
    Pos800E7104 pos;
    Dir dir;
    u8 pad9[0x15];
    u8 bits;
    u8 pad1F;
    Flags800E7104 flags;
    u8 pad24[0x52];
    u8 idle;
} Unit800E7104;

/* Relative turn applied before each of the five candidate steps. */
extern s8 D_80148330[];

void *func_800A27A4(void *out_direction, void *from, void *to);
s32 func_800A23E8(Pos800E7104 *origin, Pos800E7104 *vec);
void func_800A2F80(u8 *self, s32 step);
s32 func_800A4CC4(Unit800E7104 *unit, Pos800E7104 *pos, Dir *dir);
s32 func_800A46BC(Unit800E7104 *unit, Pos800E7104 *pos, Dir *dir);
void *func_800A2594(Pos800E7104 *out, void *from, Dir dir);
s32 func_800B56F0(void *pos);

static inline Pos800E7104 *pos_copy(Pos800E7104 *dst, Pos800E7104 *src)
{
    dst->x = src->x;
    dst->y = src->y;
    return dst;
}

static inline s32 flags_wary(Flags800E7104 *flags)
{
    return flags->wary;
}

/* Pick the step direction from `from` that brings `unit` closer to `to`;
   returns the direction (0-7) or -1 when no step is possible. */
s32 func_800E6364(Unit800E7104 *unit, Pos800E7104 *from, Pos800E7104 *to, s32 mode)
{
    Pos800E7104 next;
    Pos800E7104 goal;
    Dir dir;
    Flags800E7104 flags;
    Dir turn;
    s32 preferred = 0;
    s32 chosen;
    s32 i;
    s32 start, distance;

    func_800A27A4(&dir, from, pos_copy(&next, to));
    start = func_800A23E8(from, pos_copy(&next, to));
    distance = start;
    chosen = -1;
    for (i = 0;; i++) {
        s32 ok;
        s32 blocked;

        if (i >= 5) {
            break;
        }
        func_800A2F80(&dir.value, D_80148330[i]);
        ok = 0;
        if (func_800A4CC4(unit, from, &dir) || (mode && func_800A46BC(unit, from, &dir))) {
            ok = 1;
        }
        if (ok) {
            func_800A2594(&next, from, dir);
            goal.x = to->x;
            goal.y = to->y;
            distance = func_800A23E8(&next, &goal);
            blocked = 0;
            flags = unit->flags;
            if (flags_wary(&flags)) {
                blocked = func_800B56F0(&next) != 0;
            }
            if (blocked) {
                continue;
            }
            if (distance < start) {
                if (!mode) {
                    unit->idle = 0;
                }
                chosen = dir.value;
                break;
            }
            if (distance != start) {
                continue;
            }
            {
                Dir *tp = &turn;

                goal.x = to->x;
                goal.y = to->y;
                func_800A27A4(tp, &next, &goal);
                /* prefer a step whose follow-up direction toward the goal is even */
                if ((tp->value ^ 1) & 1) {
                    if (distance == 1) {
                        chosen = dir.value;
                        break;
                    }
                    chosen = dir.value;
                    preferred = 1;
                } else if (!preferred) {
                    chosen = dir.value;
                }
            }
        }
    }
    if (!mode) {
        if (chosen == -1) {
            unit->idle += 2;
        } else if (distance >= start) {
            unit->idle++;
        }
    }
    return chosen;
}
