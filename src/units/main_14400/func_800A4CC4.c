#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 value; } Dir;
typedef struct { s32 x, y; } Pos;
typedef Pos Tmp800A46BC;
typedef struct { u8 pad_0[9]; u8 flags_9; u8 pad_A[0x12]; u16 flags_1C; } Obj;
typedef Obj Obj800A46BC;
extern void *func_800A2594(Tmp800A46BC *out, void *arg, Dir cell);
extern u32 func_800B1C6C(Pos *pos);
extern s32 func_800A46BC(Obj800A46BC *obj, void *arg, Dir *cell);
extern s32 func_800A4314(Obj *o, Pos *p);
/* Movement check from pos in direction *dir: rejects a locked tile, a refused step, and,
 * for terrain-restricted movers (flags_1C & 8), impassable source/destination tiles and
 * diagonal steps whose two side tiles are not both passable. */
s32 func_800A4CC4(Obj *self, Pos *pos, Dir *dir) {
    Pos next;
    s32 blocked;
    s32 permitted;

    func_800A2594(&next, pos, *dir);
    blocked = 0;
    if (func_800B1C6C(pos) & 0x80) blocked = (self->flags_9 & 15) == 1;
    if (blocked) return 0;
    permitted = func_800A46BC(self, pos, dir) == 1;
    if (!permitted) return 0;
    if (self->flags_1C & 8) {
        s32 blocked_corner;
        s32 blocked_terrain = 0;

        if (!(func_800B1C6C(pos) & 0x2000) || !(func_800B1C6C(&next) & 0x2000)) blocked_terrain = 1;
        if (blocked_terrain) return 0;
        blocked_corner = 0;
        if (dir->value & 1) {
            Pos left, right;
            Dir turn, other;

            /* Diagonal step: the right-hand side tile is only looked up when the left one is
             * passable. */
            turn.value = (dir->value + 1) & 7;
            func_800A2594(&left, pos, turn);
            if (!(func_800B1C6C(&left) & 0x2000) ||
                (other.value = (dir->value - 1) & 7, func_800A2594(&right, pos, other),
                 !(func_800B1C6C(&right) & 0x2000)))
                blocked_corner = 1;
            if (blocked_corner) return 0;
        }
    }
    return func_800A4314(self, &next);
}
