#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    s32 x;
    s32 y;
} Pos;

typedef struct {
    s32 index;
    s32 f4;
} UnitIter;

/* 0x18-byte unit message passed to vtable slot 11; kind is 7 when state_52 == 3. */
typedef struct {
    s32 kind;
    u8 pad4[0x14];
} Msg;

typedef struct Unit Unit;

typedef struct {
    u8 pad0[0x58];
    short delta;
    short index;
    s32 (*fn)(void *self, Msg *msg);
} UnitVTable;

struct Unit {
    Pos pos;
    u8 pad8[0x1E - 0x8];
    u8 flags_1E;
    u8 pad1F[0x24 - 0x1F];
    UnitVTable *vt;
    u8 pad28[0x52 - 0x28];
    u8 state_52;
    u8 pad53;
    u8 flags_54;
    u8 tier_55;
    u8 moves_56;
    u8 pad57[0x5C - 0x57];
    Pos prev_5C;
    u8 pad64[0x72 - 0x64];
    u8 flags_72;
    u8 pad73[0x104 - 0x73];
    Unit *controlled_104;
};

extern unsigned short D_8014767C;
extern Unit *D_801476B8;

s32 func_800A8FC8(UnitIter *it, s32 kind);
Unit *func_800A910C(UnitIter *it);
u16 func_800E08B0(Unit *obj);
void func_800E4E24(Unit *o, s32 amount);
s32 func_800E2074(Unit *obj);
void func_800E4E90(Unit *obj);
s32 func_800A251C(Pos *x, Pos *y);
s32 func_80046240(void);
u32 func_800B1C6C(Pos *pos);
s32 func_800A7824(Unit *, u8);
s32 func_800B56F0(void *object);
void *func_800E219C(Unit *obj, void *pos);

static inline s32 has_moves(Unit *unit)
{
    return unit->moves_56 != 0;
}

static inline s32 tier_valid(s32 tier)
{
    return tier < 5;
}

/* ODD_C: the two step predicates below are inline so their pointer parameters
   bind straight to the frame addresses; the ROM materializes &from/&to at these
   calls and keeps only the blocking checks' &to in a loop-hoisted register. */

/* Nonzero when the unit's step changed its position. */
static inline s32 has_moved(Pos *from, Pos *to)
{
    return func_800A251C(from, to) ^ 1;
}

/* Tile attribute bit 0x80 at the position (ends a step early). */
static inline u32 is_stop_tile(Pos *pos)
{
    return func_800B1C6C(pos) & 0x80;
}

static inline s32 is_controlled(Unit *unit)
{
    Unit *controlled = D_801476B8->controlled_104;

    return controlled != 0 && unit == controlled;
}

/* Step every eligible unit, tier by tier (low nibble of +0x55), while it has moves left. */
void func_800C8440(unsigned short speed, unsigned short turns /* supplied, unused */)
{
    Msg msg;
    Pos from;
    Pos to;
    UnitIter it;
    s32 tier;
    Unit *u;
    s32 skip;

    if ((D_8014767C >> 6) & 1) {
        return;
    }
    it.index = 0;
    for (tier = 0; tier_valid(tier); tier++) {
        it.index = 0;
        while (func_800A8FC8(&it, 0x7C)) {
            u = func_800A910C(&it);
            skip = 0;
            if (!func_800E08B0(u)) {
                skip = 1;
            } else if ((u->tier_55 & 0xF) != tier) {
                skip = 1;
            } else if (((u->flags_1E >> 2) & 1) && D_801476B8->controlled_104 == 0) {
                skip = 1;
            } else if (is_controlled(u)) {
                skip = 1;
            }
            if (skip) {
                continue;
            }
            msg.kind = 0;
            if (u->state_52 != 0) {
                if ((u->state_52 ^ 3) != 0) {
                    continue;
                }
                msg.kind = 7;
            }
            func_800E4E24(u, speed);
            if (u->flags_72 & 4) {
                u->flags_72 &= ~4;
                u->moves_56 = 0;
            }
            while (has_moves(u)) {
                skip = 0;
                if (!func_800E08B0(u)) {
                    skip = 1;
                } else if (!func_800E2074(u)) {
                    skip = 1;
                }
                if (skip) {
                    break;
                }
                from.x = u->pos.x;
                from.y = u->pos.y;
                if ((u->vt->fn((u8 *)u + u->vt->delta, &msg) ^ 1) != 0) {
                    break;
                }
                func_800E4E90(u);
                to.x = u->pos.x;
                to.y = u->pos.y;
                if (!has_moved(&from, &to)) {
                    continue;
                }
                u->flags_54 |= 2;
                u->prev_5C = from;
                if ((func_80046240() ^ 1) == 0) {
                    continue;
                }
                if (is_stop_tile(&to)) {
                    u8 stopped = func_800A7824(u, 2);
                    if (stopped) {
                        break;
                    }
                    continue;
                }
                {
                    s32 blocked = func_800B56F0(&to) || func_800E219C(u, &to);
                    if (blocked) {
                        u->moves_56 = 0;
                        break;
                    }
                }
            }
        }
    }
}
