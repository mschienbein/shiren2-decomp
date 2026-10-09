#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef struct { s32 x, y; } Vec2;
typedef struct { s8 d; } Dir;
/* Partial unit view: the position is followed by direction, elevation and
 * effect-state bytes; the last accessed byte is +0x75. */
typedef struct {
    Vec2 pos;
    u8 direction;
    s8 elevation;
    u8 pad0A[0x14], flags1E, kind;
    u8 pad20[0x55], power;
} Unit;
typedef struct { u8 pad[0x104]; s32 field104; } Player;
typedef struct { u8 kind, id; } Item;
typedef struct {
    u8 pad00[0x12]; u16 flags12;
    u8 pad14[8]; s32 field1C, field20, field24, field28, field2C;
} Task;
/* Equipment descriptor (4 bytes): +0 model/tile id (lhu), +2 percentage
 * byte (lbu in func_80077C4C), +3 never accessed. */
typedef struct { u16 id; u8 percent; u8 unk3; } EquipInfo;
typedef struct { u8 kind, variant, field02, flags, row, field05, field06, field07, mode; s8 coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
extern s32 D_80139334[], D_8013937C[], D_801393C4[];
extern u32 D_801393E4[], D_801394B0[];
extern u16 D_8013957C[];
extern s32 D_80139688, D_80139694, D_801E4E70;
extern u32 D_8013968C;
extern EquipInfo D_8013F2EC[];
extern Player *D_801476B8;
extern s32 func_800419C4(void);
extern s32 func_80042990(void);
extern s32 func_80048AE4(void);
extern s32 func_80048B04(Vec2 *);
extern s32 func_80048EE0(Unit *);
extern void func_80048FD8(void *);
extern void func_80049414(void *, u32 *, s32 *);
extern void *func_80050FA0(s32, Unit *, s32, s32);
extern void *func_800510E0(s32, Unit *, s32, s32, s32);
extern void func_80051264(s32, void *, s32);
extern void func_800512BC(s32, Unit *, s32, s32, s32);
extern s32 func_800625FC(s32, s32);
extern s32 func_800627C4(void);
extern s32 func_80083FF8(void);
extern void func_800840B0(s32);
extern void func_80084A20(void);
extern void func_80084A68(void);
extern s32 func_80084AD8(s32, void (*)(void *));
extern void func_80084B80(void);
extern s32 func_80084CB8(void);
extern void func_800850F8(void (*)(void *), u16);
extern void *func_80085154(void (*)(void *), s32);
extern void *func_800851B0(s32);
extern void func_800855E0(void *);
extern void func_800874FC(void *);
/* The callback consumes only a0 (task), and returns no value. */
extern void func_80087E4C(void *);
extern void func_8008865C(void *);
extern void func_8008B678(void *);
extern void func_8008B9B0(void *);
extern void *func_800A2594(Vec2 *, void *, Dir);
extern Vec2 *func_800A25D8(Vec2 *, Vec2 *, Dir, s32);
extern s32 func_800A6FD0(void *);
extern u8 func_800A8C00(void *);
extern void *func_800B4928(Vec2 *);
extern Unit *func_800C5F60(void);
extern s32 func_800E04D0(void *);
extern s32 func_800E49F4(void *, s32);
extern Item *func_800E8978(Unit *);
extern void *func_800E8A68(void *, u8);

static inline void copy(Vec2 *dst, const Vec2 *src) {
    dst->x = src->x;
    dst->y = src->y;
}
static inline Dir direction(Unit *u) { Dir d; d.d = u->direction; return d; }
static inline void refresh_player(void) {
    func_80084A20();
    func_800850F8(func_8008865C, (func_80083FF8() * 2) & 0xFFFE);
    func_80048FD8(func_800C5F60());
    func_80084B80();
}
/* ODD_C: Keep the packed selector's halfword conversion distinct from the
 * effect identifier; the inline accessor also shapes the switch operand. */
static inline u16 effect_selector(s32 descriptor) {
    return descriptor & 0xF800;
}
/* ODD_C: An explicitly narrow predicate retains the original compare form. */
static inline u8 differs_from_one(s32 value) { return value != 1; }
/* ODD_C: Isolate the conditional terrain predicate before the shared wet
 * assignment; this also preserves the two-stage branch structure. */
static inline s32 terrain_is_flooded(u32 terrain) {
    s32 flooded = 0;
    if ((terrain & 0x20100000) && func_800627C4() == 3)
        flooded = (D_80142F18.flags & 3) == 2;
    return flooded;
}
/* ODD_C: Promote the power byte before switching, preserving a signed index. */
static inline s32 unit_power(Unit *u) { return u->power; }

/* ODD_C: A narrow predicate keeps the zero operand in the compare. */
static inline u8 nonzero(s32 value) { return value != 0; }
void func_8004D588(Unit *u, s32 a, s32 b) {
    Vec2 start, pos, next;
    u32 taskFlags;
    s32 taskMode;
    s32 mode, extraFlags, finalFlags, wet;
    s32 skip, actorIndex, flags, count, distance, i, kind;
    s32 motion;
    u32 packed;
    s32 itemId;
    u8 unitKind;
    Item *item;
    Unit *other;
    Task *task;

    actorIndex = func_800A8C00(u);
    copy(&start, &u->pos);
    if (D_80139694 == 0 && func_80048B04(&start) == 0) {
        s32 skip = 1;
        if (D_8013968C == 0x47) {
            s32 steps = a;
            if (a <= 0) steps = 1;
            func_800A25D8(&next, &start, direction(u), steps);
            pos = next;
            if (func_80048B04(&pos) == 1) skip = 0;
        }
        if (skip) return;
    }
    skip = 0;
    if (D_8013968C == 0x21 && func_80048EE0(u)) skip = func_800A6FD0(u) == 0;
    if (skip) D_8013968C = 0x22;
    switch (D_8013968C) {
    case 0x21:
    case 0x22:
    case 0x23: {
        s32 id;
        if (b & 0x80) {
            id = 0xB5;
            if (u->flags1E & 0xC) id = 7;
        } else if (b & 0x100) {
            id = 0xB6;
            if (u->flags1E & 0xC) id = 8;
        } else {
            id = 0xB4;
            if (u->flags1E & 0xC) id = 6;
        }
        if (D_8013968C == 0x22) {
            task = func_800851B0(id);
            task->field1C = 2;
            func_800850F8(func_8008865C, 6);
            return;
        }
        if (func_80084AD8(func_80084CB8(), func_80087E4C)) {
            if (D_8013968C == 0x23) return;
            task = func_800851B0(id);
            task->field1C = 2;
            func_800850F8(func_8008865C, 6);
            return;
        }
        if (D_8013968C != 0x23) {
            func_80084A20();
            task = func_800851B0(id);
            task->field1C = 2;
            func_80084B80();
        }
        if ((u->flags1E >> 2) & 1) {
            if (D_801476B8->field104) func_80048FD8(D_801476B8);
            if (b & 0x4000) {
                func_80084A20();
                func_80050FA0(0x16C, u, u->direction, 0x10000);
                func_80084B80();
                func_80051264(0x15A, u, 0x10000);
                if (D_801476B8->field104) refresh_player();
                return;
            }
        }
        task = func_80085154(func_800855E0, actorIndex);
        task->field2C = a;
        if (b & 0xC00) task->field24 = 1;
        else if (b & 0x1000) task->field24 = 2;
        else if (b & 0x2000) task->field24 = 3;
        else if (b & 0x8000) task->field24 = 4;
        else task->field24 = 0;
        if ((u->flags1E & 0x7C) && a == 0) {
            func_80049414(u, &taskFlags, &taskMode);
            task = func_80085154(func_8008B9B0, actorIndex);
            task->field24 = taskFlags;
            task->field28 = taskMode;
            {
                s32 refresh = 0;
                if ((u->flags1E >> 2) & 1) refresh = nonzero(D_801476B8->field104);
                if (refresh) refresh_player();
            }
        }
        break;
    }
    case 0x47:
    case 0x48:
    case 0x69:
    case 0x6E:
        flags = 0;
        extraFlags = 0;
        finalFlags = 0;
        count = 0;
        pos.x = u->pos.x;
        pos.y = u->pos.y;
        distance = a;
        if (D_8013968C != 0x6E && D_8013968C != 0x48 && func_800A6FD0(u)) {
            for (i = 1; ; i++) {
                Dir step;
                if (i > a) break;
                step = direction(u);
                func_800A25D8(&next, &pos, step, i);
                if (func_800B4928(&next)) { count++; break; }
            }
            if (D_8013968C == 0x69) { distance = 1; count++; }
            if (D_80139688 == 0) count = 0;
            else if (differs_from_one(func_80048AE4())) count = 0;
            if (count) {
                s32 update, quiet, busy;
                quiet = 0;
                busy = 0;
                if (D_801E4E70 == 0 || func_800419C4() || func_80042990()) busy = 1;
                if (busy) quiet = 1;
                update = func_800E49F4(u, quiet);
                flags |= 0x2001;
                func_800840B0(update);
            }
        }
        unitKind = u->kind;
        if (unitKind == 0xD5) {
            flags |= 0x10000;
            func_80051264(0x131, u, flags);
            return;
        } else if (unitKind == 0xD6) {
            flags |= 0x10000;
            func_80051264(0x137, u, flags);
            return;
        }
        kind = unitKind;
        mode = u->direction;
        switch (kind) {
        case 0x17:
        case 0x1A:
        case 0x5B:
        case 0x5C: {
            /* ODD_C: These item-effect flags have independent lifetimes from
             * the incoming distance and action flags; sharing a/b worsens
             * saved-register allocation even though the values are disjoint. */
            s32 actorIndex, count, launchFlags, motionFlags;
            u32 terrain;
            actorIndex = 0;
            terrain = func_800625FC(pos.y, pos.x);
            if ((terrain & 0x4000) || terrain_is_flooded(terrain)) wet = 1;
            else wet = 0;
            launchFlags = 0x20000;
            if (kind != 0x5C) launchFlags = 0x10000;
            motionFlags = 0x10000;
            if (((u->flags1E >> 4) & 1) || kind == 0x5B || kind == 0x5C) {
                itemId = 0x59;
            } else {
                item = func_800E8978(u);
                if (item == 0) itemId = 0x59;
                else switch (item->kind) {
                case 4:
                    itemId = 0x31;
                    if (item->id == 0x6D) itemId = 0x5A;
                    break;
                case 9:
                    itemId = 0x5B;
                    break;
                default:
                    itemId = item->id;
                    if (D_8013968C == 0x69 && itemId == 0x49) itemId = 0x5C;
                    else if (D_8013968C == 0x69 && itemId == 0x58) itemId = 0x5D;
                    else if (D_8013968C == 0x48 && itemId == 0x44) {
                        itemId = 0x5E;
                        launchFlags |= 0x800;
                        finalFlags = 0x400;
                        motionFlags |= 0x800;
                    } else if (itemId == 0x4C) {
                        itemId = distance + 0x5E;
                        launchFlags |= 0x800;
                        finalFlags = 0x400;
                        motionFlags |= 0x800;
                    } else if (itemId == 0x45) {
                        launchFlags |= 0x800;
                        extraFlags = 0x800;
                        finalFlags = 0x400;
                        motionFlags |= extraFlags;
                    } else if (wet && itemId == 0x4D) itemId = 0x62;
                    else if (itemId == 0x4E) {
                        func_800A2594(&next, &pos, direction(u));
                        other = func_800B4928(&next);
                        if (other && (u->elevation & 0xF) > (other->elevation & 0xF)) itemId = 0x64;
                    }
                    break;
                }
                if (itemId < 0x32) itemId = 0x63;
                {
                    Item *held = func_800E8A68(u, 4);
                    actorIndex = 0;
                    if (held) actorIndex = held->id;
                }
            }
            if (kind == 0x17 || kind == 0x5B || kind == 0x5C) packed = D_801393E4[itemId - 0x32];
            else packed = D_801394B0[itemId - 0x32];
            distance = D_80139334[(packed >> 16) & 0x1F];
            if (actorIndex) count = D_801393C4[(packed >> 21) & 7];
            else count = 0;
            motion = D_8013937C[(packed >> 16) & 0x1F];
            packed &= 0x7FFF;
            func_80084A68();
            func_800850F8(func_8008865C, 0);
            func_80084A20();
            func_80051264(distance, u, flags | launchFlags);
            func_80084B80();
            func_80084A20();
            func_80050FA0(motion, u, mode, motionFlags);
            func_80084B80();
            if (wet) flags |= 0x40;
            if (count) {
                motion = D_8013F2EC[actorIndex - 0x59].id;
                func_80084A20();
                func_800510E0(count, u, mode, flags | extraFlags | 0x10000, motion);
                func_80084B80();
            }
            if (packed) func_800510E0(packed, u, mode, flags | finalFlags | 0x10000, 0);
            return;
        }
        default: {
            s32 id;
            u16 selector;
            if ((u32)(kind - 0x18) < 0x47) {
            if (kind == 0x1B) flags |= 0x40000;
            else if ((u32)(kind - 0x18) < 5) flags |= 0x10000;
            id = D_8013957C[kind - 0x18];
            selector = effect_selector(id);
            id &= 0x7FF;
            switch (selector) {
            case 0x0800:
                if (a >= 0) {
                    id = 0xDA;
                    if (a >= 2) { id = 0xDC; if (a == 2) id = 0xDB; }
                } else id = 0xDC;
                break;
            case 0x1800:
                switch (unit_power(u)) {
                case 1: id = 0xD4; break;
                case 2: id = 0xD6; break;
                case 3: id = 0xD8; flags |= 0x10000; break;
                default: id = 0xD8; flags |= 0x20000; break;
                }
                break;
            case 0x1000:
                if (u->power != 1) {
                    func_800A2594(&next, &pos, direction(u));
                    other = func_800B4928(&next);
                    if (other && (u->elevation & 0xF) < (other->elevation & 0xF)) id = 0xBA;
                }
                break;
            case 0x2800: {
                s32 step = u->power - 1;
                if (step >= 3) step = 2;
                func_80051264(id + step, u, flags);
                return;
            }
            case 0x3000: {
                s32 step = u->power - 1;
                if (step >= 4) step = 3;
                func_80051264(id + step, u, flags);
                return;
            }
            case 0x3800:
                if (b & 0x180) id = 0xC5;
                break;
            case 0x4000: {
                s32 power = u->power;
                s32 level;
                if (power < 0x14) level = 0;
                else if (power < 0x1E) level = 1;
                else if (power < 0x3C) level = 2;
                else if (power < 0x46) level = 3;
                else if (power < 0x4D) level = 4;
                else if (power < 0x58) level = 5;
                else if (power < 0x5A) level = 6;
                else if (power < 0x63) level = 7;
                else level = 8;
                func_800512BC(id, u, flags, level, 0);
                return;
            }
            case 0x4800:
                func_800A2594(&next, &pos, direction(u));
                other = func_800B4928(&next);
                if (other && (u->elevation & 0xF) < (other->elevation & 0xF)) id = 0xB5;
                break;
            case 0x5000:
                switch (unit_power(u)) {
                case 2: id = 0x11D; break;
                case 3: id = 0x11E; break;
                }
                break;
            case 0x5800:
                switch (unit_power(u)) {
                case 1: id = 0xCA; break;
                case 2: id = 0xCB; break;
                /* ODD_C: power 3 is a real level that shares the default identifier;
                 * its explicit label also keeps the original compare layout. */
                case 3:
                default: id = 0xCC; break;
                }
                break;
            case 0x6000: motion = 0x10F; goto emitMotion;
            case 0x6800: motion = 0x110; goto emitMotion;
            case 0x7000: motion = 0x111; goto emitMotion;
            case 0x8000: motion = 0x115; goto emitMotion;
            case 0x7800: motion = 0x112;
emitMotion:
                func_80084A68();
                func_800850F8(func_8008B678, 0);
                func_80084A20();
                func_80050FA0(motion, u, u->direction, flags);
                func_80084B80();
                break;
            case 0x2000: flags |= 0x20000; break;
            }
            func_80051264(id, u, flags);
            return;
            }
                func_800851B0(0xB3);
                task = func_80085154(func_800874FC, actorIndex);
                task->flags12 = flags;
                return;
        }
        }
    case 0xA6: {
        s32 id = -1;
        if (a == 0) {
            if (!func_800A6FD0(u)) return;
            id = 0x32;
        } else if (b == 0) {
            switch (a) {
            case 4: id = 0x8E; break;
            case 2: id = 0x31; break;
            case 6: id = 0x37; break;
            case 10:
            case 11: id = 0x2F; break;
            case 12: id = 0x2E; break;
            case 14: id = 0x2D; break;
            case 18:
                if (!(u->flags1E & 0x7C)) return;
                if ((func_800E04D0(u) & 0xFF) == 1) return;
                id = 0x33;
                break;
            case 19: id = 0x38; break;
            }
        }
        func_800851B0(id);
        break;
    }
    }
}
