#include "common.h"

typedef unsigned char u8;
typedef struct { s32 x, y; } Cell;
typedef struct { u8 v; } Dir;
typedef struct Obj Obj;
extern u8 D_80147620[];
extern u8 func_800C57A0(void *rng);
extern void *func_800A2594(Cell *out, void *arg, Dir direction);
extern void func_800A2758(Cell *pos, Dir direction);
extern s32 func_800B43BC(void *pos, s32 value, u8 flags);
extern u32 func_800B1C6C(Cell *pos);
extern s32 func_801368E4(Cell *pos);

static inline Dir *set_direction(Dir *out, s32 value)
{
    out->v = value & 7;
    return out;
}

static inline s32 blocked(Cell *cell)
{
    return func_800B1C6C(cell) & 0x800;
}

static inline s32 traversable(Cell *cell)
{
    s32 valid = 0;
    if (func_801368E4(cell) && !(func_800B1C6C(cell) & 0x1000) && !blocked(cell)) {
        valid = 1;
    }
    return valid;
}

/* `unused` is the caller-supplied owner object (func_800BC49C passes it); this
 * wall-follower never reads it. */
void func_800BC2AC(Obj *unused, Cell *source, u8 direction)
{
    Cell current, next;
    /* ODD_C: cursor pointers to the two cells used by the probing calls; the
     * original mixes them with direct &current, which also shapes allocation. */
    Cell *cur;
    Cell *nxt;
    Dir initial, step, back, turn, other;
    s32 visits;
    s32 stuck;
    s32 random = func_800C57A0(D_80147620) & 1;
    s32 left = random ? 2 : 6;
    s32 right = random ? 6 : 2;
    func_800A2594(&current, source, *set_direction(&initial, left + direction));
    visits = 0;
    cur = &current;
    nxt = &next;
    for (;;) {
        func_800B43BC(&current, 0, 0);
        if (blocked(cur)) {
            visits++;
            if (visits == 2) break;
        }
        func_800A2758(cur, *set_direction(&step, direction));
        if (traversable(cur)) {
            func_800A2758(&current, *set_direction(&back, direction + 4));
            direction += left;
            func_800A2594(nxt, &current, *set_direction(&turn, direction));
            if (!(func_800B1C6C(nxt) & 0xE100)) break;
        } else {
            func_800A2594(nxt, &current, *set_direction(&other, right + direction));
            stuck = traversable(nxt) != 1;
            if (stuck) direction += right;
        }
        direction &= 7;
    }
}
