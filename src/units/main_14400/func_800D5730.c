#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    s32 x;
    s32 y;
} Pos;

typedef struct {
    u8 value;
} Dir;

void *func_800A2594(Pos *out, void *src, Dir dir);
u32 func_800B1C6C(Pos *pos);

static inline Dir turned(u8 *dir, s32 turn)
{
    Dir result;

    result.value = (*dir + turn) & 7;
    return result;
}

/* Wall bits of the square next to `pos` in direction `dir` turned by `turn`. */
static inline u32 side_wall(Pos *side, Pos *pos, u8 *dir, s32 turn)
{
    func_800A2594(side, pos, turned(dir, turn));
    return func_800B1C6C(side) & 0xE100;
}

static inline u32 tile_bits(Pos *p, u32 mask)
{
    return func_800B1C6C(p) & mask;
}

/* Tile attribute bits; every attribute mask fits in 16 bits. */
static inline u16 attr_bits(u32 flags, u16 mask)
{
    return flags & mask;
}

static inline s32 has_bit(u32 value, u32 mask)
{
    return (value & mask) != 0;
}

/* Whether a unit at `pos` may step forward in `dir`: the front square must be open,
   no diagonal may be cut around a wall corner, and the water state must not change
   into a deep square. */
s32 func_800D5730(Pos *pos, u8 *dir)
{
    Pos front;
    Pos left2;
    Pos left1;
    Pos left3;
    Pos right2;
    Pos right1;
    Pos right3;
    Dir facing;
    Pos *probe;
    Pos *ahead;
    s32 here;
    s32 there;
    s32 hit;
    u32 flags;
    s32 blocked;

    facing.value = *dir;
    func_800A2594(&front, pos, facing);
    blocked = 0;
    if (tile_bits(&front, 0xE100) || tile_bits(&front, 0x80)) {
        blocked = 1;
    } else if (!side_wall(probe = &left2, pos, dir, 2)
               && (side_wall(probe = &left1, pos, dir, 1) || side_wall(probe = &left3, pos, dir, 3))) {
        blocked = 1;
    } else if (!side_wall(probe = &right2, pos, dir, -2)
               && (side_wall(probe = &right1, pos, dir, -1) || side_wall(probe = &right3, pos, dir, -3))) {
        blocked = 1;
    }
    if (blocked) {
        return 0;
    }
    /* Moving between a water square and a deep square is refused. */
    here = has_bit(func_800B1C6C(pos), 0x1000);
    ahead = &front;
    flags = func_800B1C6C(ahead);
    hit = 0;
    there = attr_bits(flags, 0x1000) != 0;
    if (!here ? (func_800B1C6C(pos) & 0x800) && there : !there && (func_800B1C6C(ahead) & 0x800)) {
        hit = 1;
    }
    return hit ^ 1;
}
