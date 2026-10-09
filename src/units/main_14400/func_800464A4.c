#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct { s32 x; s32 y; } Pos800464A4;
typedef struct { s32 x0; s32 y0; s32 x1; s32 y1; } Rect800464A4;
typedef struct { Pos800464A4 cur; Pos800464A4 origin; Pos800464A4 end; } Iter800464A4;

typedef struct {
    unsigned high : 8;
    unsigned bit23 : 1;
    unsigned middle : 15;
    unsigned bit7 : 1;
    unsigned low : 7;
} Flags800464A4;

typedef struct {
    u8 pad0[0x20];
    Flags800464A4 flags_20;
} Ctx800464A4;

typedef struct {
    u8 pad0[0xE4];
    u16 flags_E4;
    u8 padE6[0x1E];
    void *field_104;
} Player800464A4;

typedef struct {
    u8 pad0[0xA];
    u8 kind_A;
    u8 padB[0x13];
    u8 flags_1E;
    u8 pad1F[0x7B];
    u16 flags_9A;
} Unit800464A4;

typedef struct {
    u8 kind_0;
    u8 id_1;
    u8 flags_2;
} Item800464A4;

extern u8 D_80143391;
extern u16 D_8014767C;
extern Player800464A4 *D_801476B8;
typedef struct { void *room; s32 kind, changed, active; } RoomState;
extern RoomState D_80143434;
extern Rect800464A4 D_801429C0;

Ctx800464A4 *func_800C5F60(void);
s32 func_800C965C(void);
/* Returns its rectangle by value through the hidden result pointer. */
Rect800464A4 func_800B3024(Ctx800464A4 *ctx);
Pos800464A4 *func_800A3610(Pos800464A4 *out, Iter800464A4 *iter);
u32 func_800B1C6C(Pos800464A4 *pos);
Unit800464A4 *func_800B4928(Pos800464A4 *pos);
s32 func_800E1CC4(Unit800464A4 *unit, s32 flag);
s32 func_800A24DC(Pos800464A4 *pos, Rect800464A4 *rect);
Item800464A4 *func_800B4D80(Pos800464A4 *pos);
s32 func_800B5728(Pos800464A4 *pos);

/* ODD_C: Access the copied flag objects using the canonical bitfield view;
 * the accessor boundary preserves the original whole-word copy lowering. */
static inline s32 flag23(Flags800464A4 *flags) { return flags->bit23; }
static inline s32 flag7(Flags800464A4 *flags) { return flags->bit7; }

/* ODD_C: Copy members in source order rather than using aggregate assignment. */
static inline void copy_rect(Rect800464A4 *out, const Rect800464A4 *in) {
    s32 x0 = in->x0, y0 = in->y0, x1 = in->x1, y1 = in->y1;
    out->x0 = x0; out->y0 = y0; out->x1 = x1; out->y1 = y1;
}
/* ODD_C: Preserve the separate flag-test expression used by the unit view. */
static inline s32 unit_hidden(Unit800464A4 *unit) {
    return (unit->flags_1E >> 1) & 1;
}

/* ODD_C: Express the comparison as a byte-valued predicate rather than a dead
 * flag assignment, preserving the original xori/zero-test lowering. */
static inline u8 not_one(s32 v) { return v != 1; }

/* The caller passes the map-renderer object D_80138C40; it is unused here. */
s32 func_800464A4(void *unused, u32 (*grid)[54], Rect800464A4 *rect) {
    Rect800464A4 area;
    Rect800464A4 view;
    Iter800464A4 iter;
    Pos800464A4 pos;
    Flags800464A4 ctxFlags;
    Flags800464A4 curFlags;
    s32 bit23;
    s32 haveRoom;
    s32 forced;
    s32 hidden;
    s32 blind;
    s32 special;
    s32 visible;
    u32 flags;
    u32 keep;
    u32 marked;
    s32 changed;
    u32 *cell;
    Unit800464A4 *unit;
    Unit800464A4 *u;
    Item800464A4 *item;
    Ctx800464A4 *ctx;

    ctxFlags = func_800C5F60()->flags_20;
    bit23 = flag23(&ctxFlags);
    forced = D_80143391 & 1;
    haveRoom = func_800C965C();
    curFlags = func_800C5F60()->flags_20;
    hidden = 0;
    if (flag7(&curFlags) || ((D_8014767C >> 5) & 1)) {
        hidden = 1;
    }
    blind = 0;
    if ((D_801476B8->flags_E4 >> 3) & 1) {
        blind = D_801476B8->field_104 == 0;
    }
    ctx = func_800C5F60();
    area.x0 = rect->x0;
    area.y0 = rect->y0;
    area.x1 = rect->x1;
    area.y1 = rect->y1;
    changed = 0;
    if (D_80143434.active != 0) {
        copy_rect(&view, &D_801429C0);
    } else {
        view = func_800B3024(ctx);
    }
    pos.x = area.x0;
    pos.y = area.y0;
    iter.origin = pos;
    iter.cur = iter.origin;
    pos.x = area.x1;
    pos.y = area.y1;
    iter.end = pos;
    for (;;) {
        s32 more = iter.cur.x <= iter.end.x;

        if (!more) {
            break;
        }
        func_800A3610(&pos, &iter);
        cell = &grid[pos.y][pos.x];
        keep = *cell & 0x3F7F0000;
        marked = *cell & 0x800000;
        flags = (u16)func_800B1C6C(&pos);
        if ((flags & 0x400) || forced) {
            if (flags & 0x2100) {
                flags |= 0x20000;
            } else if (!(flags & 0xC000)) {
                flags |= 0x10000;
            }
        }
        unit = func_800B4928(&pos);
        /* ODD_C: u is the flag view of the cell's unit taken before the null
         * test; the flag tests read it while the visibility calls use unit.
         * It reproduces the original's separate copy of the unit pointer. */
        u = unit;
        if (unit != 0) {
            if ((u->flags_1E >> 2) & 1) {
                flags |= 0x40000;
            } else {
                special = 0;
                if (((u->flags_1E >> 3) & 1) || u->kind_A == 0x5B || u->kind_A == 0x5C ||
                    (((u->flags_1E >> 4) & 1) && (u->flags_9A & 0x40))) {
                    special = 1;
                }
                if (special) {
                    flags |= 0x4000000;
                } else if (unit_hidden(u)) {
                    if (hidden || (flags & 0x400) || forced == 1) {
                        flags |= 0x2000000;
                    }
                } else if (forced == 1) {
                    flags |= 0x80000;
                } else if (haveRoom && not_one(func_800E1CC4(unit, 1))) {
                    flags |= 0x80000;
                } else {
                    visible = 0;
                    if (func_800A24DC(&pos, &view) && (blind || func_800E1CC4(unit, 1) == 0)) {
                        visible = 1;
                    }
                    if (visible) {
                        flags |= 0x80000;
                    }
                }
            }
        }
        item = func_800B4D80(&pos);
        if (item != 0) {
            switch (item->kind_0) {
            case 0x10:
                if (((flags & 0x400) && (!(item->flags_2 & 0x10) || blind || bit23)) || (bit23 && hidden) || forced == 1) {
                    flags |= 0x200000;
                }
                if (!(item->flags_2 & 0x10) || blind || bit23) {
                    flags |= 0x8000000;
                }
                break;
            case 0xF:
                if (((flags & 0x400) && !(item->flags_2 & 0x10) && !(flags & 0x4000)) || forced == 1) {
                    if (item->id_1 != 0xCF) {
                        flags |= 0x100000;
                    } else {
                        flags |= 0x20000000;
                    }
                }
                break;
            case 0x13:
                if (forced == 1) {
                    flags |= 0x10000000;
                } else if (item->id_1 == 0xF2 && (hidden || (flags & 0x400)) && !(item->flags_2 & 0x10)) {
                    flags |= 0x400000;
                }
                break;
            default:
                if (((hidden || (flags & 0x400)) && !(item->flags_2 & 0x10)) || forced == 1) {
                    flags |= 0x400000;
                }
                break;
            }
        } else if ((u8)func_800B5728(&pos)) {
            flags |= 0x1000000;
        }
        if ((flags & 0x3FFF0000) != keep) {
            *cell = flags | 0x800000;
            changed = 1;
        } else {
            *cell = flags | marked;
        }
    }
    return changed;
}
