#include "common.h"
#include "map_types.h"

/*
 * Knock a unit back along a direction (C++ TU, g++ 2.8.1; the position and
 * direction value types are MapPos/MapDir of map_types.h, the same family as
 * func_800A694C/func_800E7794).  Evidence for C++: MapPos has a
 * user copy constructor, so start/cur/next and the step result live in
 * memory and are assigned with the implicit block copy; MapDir::turned() keeps
 * `face` addressable (its receiver pointer is the loop-hoisted sp+0x48);
 * the flag tests go through inline members of a stack copy of the flags
 * word (the dead stores at sp+0x50/0x54); the effect record is built by an
 * inline constructor around func_80136910, so &fx is substituted directly
 * rather than held in a register; the unit's methods are g++ vtable slots.
 */

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

/* Unit vtable view (g++ 2.x entries: this delta, index, function); only the +0x10 and +0x18
   entries are typed. */
struct UnitVTableView_800A7204 {
    u8 pad00[0x10];
    short delta_10;
    short index_12;
    s32 (*isGone_14)(void *self);
    short delta_18;
    short index_1A;
    void (*vanish_1C)(void *self);
};

/* View of the unit flag word at +0x20 (bit 25 silent, bit 13 reflects, MSB-first). */
struct FlagsView_800A7204 {
    u32 pad0 : 6;
    u32 silent : 1;
    u32 pad7 : 11;
    u32 reflects : 1;
    u32 pad19 : 13;
    bool canReflect() const { return reflects; }
    bool isSilent() const { return silent; }
};

struct Effect_800A7204;
extern "C" void func_80136910(Effect_800A7204 *obj, void *source, u32 value, u32 type, u32 flags);
/* Hit effect record (0x14 bytes), initialised by func_80136910. */
struct Effect_800A7204 {
    void *field_0;
    u32 field_4;
    u32 field_8;
    u16 field_C;
    u16 field_E;
    u8 field_10;
    Effect_800A7204(void *source, s16 value) { func_80136910(this, source, value, 0x22, 0x808); }
};

/* Partial view of the unit record. Common prefix with the other b7 unit views: MapPos at +0,
   the +0x1E status byte and the +0x24 vtable (UnitView_800C37B8, UnitView_800C62B8); the
   +0x20 flag word is also viewed by ObjectView_800E11C0 and PosFlagsView_8011C5C0. */
struct UnitView_800A7204 {
    MapPos pos;        /* 0x00 */
    u8 pad8;
    u8 kind;        /* 0x09 */
    u8 padA[0x12];
    u16 state;      /* 0x1C */
    u8 status;      /* 0x1E */
    u8 pad1F;
    FlagsView_800A7204 flags;    /* 0x20 */
    UnitVTableView_800A7204 *vt; /* 0x24 */
    s32 isGone() { return vt->isGone_14((char *)this + vt->delta_10); }
    void vanish() { vt->vanish_1C((char *)this + vt->delta_18); }
};

extern "C" {
u32 func_800B1C6C(MapPos *pos);
void func_800A7B18(void *target, void *source, s32 amount, s32 kind);
void func_800A59A4(UnitView_800A7204 *self);
void func_800A665C(UnitView_800A7204 *obj, MapDir *dir);
s32 func_80049CB4(s32 id, ...);
void func_800A2758(MapPos *p, MapDir d);
s32 func_800A4754(UnitView_800A7204 *obj, MapPos *pos, MapDir *dir);
s32 func_800A4404(UnitView_800A7204 *unit, MapPos *pos, void **hit);
s32 func_800B56F0(void *object);
char *func_800A3B20(UnitView_800A7204 *u);
void func_800497F0(s32, ...);
u16 func_800B5768(void *object);
void func_800A7ADC(UnitView_800A7204 *unit, Effect_800A7204 *effect);
u16 func_800E08B0(void *obj);
void func_800A0600(void);
void func_800A0618(UnitView_800A7204 *unit);
s32 func_800A4314(UnitView_800A7204 *o, MapPos *p);
void func_800A58FC(void *actor, MapPos *position);
void *func_800A2594(MapPos *out, void *from, MapDir dir);
void func_800A2F94(MapDir *dir, s32 steps);
s32 func_800A60D8(UnitView_800A7204 *unit, MapPos *pos);
s32 func_800A5168(UnitView_800A7204 *unit, MapPos *pos, s32 alternate);
void func_800A7BA4(UnitView_800A7204 *unit, s32 arg);
}

extern "C" void func_800A7204(UnitView_800A7204 *u, void *attacker, MapDir *facing, s32 range, s32 damage, s32 kind,
                              s32 checkWall, s32 notify)
{
    if (u->state & 1) {
        return;
    }
    s32 immobile = 0;
    if ((u->state & 2) || ((func_800B1C6C(&u->pos) & 0x80) && (u->kind & 0xF) == 1)) {
        immobile = 1;
    }
    if (immobile) {
        if (damage) {
            func_800A7B18(u, attacker, damage, kind);
        }
        return;
    }
    UnitView_800A7204 *target = 0;
    MapPos start(u->pos);
    s32 bounced = 0;
    MapDir face(*facing);
    MapPos cur(start);
    MapPos next(start);
    s32 origKind = u->kind & 0xF;
    s32 sound = kind == 7 ? 0x18 : 0x17;
    func_800A59A4(u);
    MapDir back(face.turned(4));
    func_800A665C(u, &back);
    func_80049CB4(0x1131);
    s32 moved = 0;
    s32 steps = range;
    while (--steps != -1) {
        func_800A2758(&next, face);
        s32 blocked = 0;
        void *hit = 0;
        if (!func_800A4754(u, &cur, &face)) {
            blocked = 1;
        } else if (!func_800A4404(u, &next, &hit)) {
            blocked = 1;
        } else if (checkWall && (func_800B1C6C(&next) & 0x2000)) {
            blocked = 1;
        }
        if (blocked) {
            if (hit != 0) {
                FlagsView_800A7204 hitFlags(((UnitView_800A7204 *)hit)->flags);
                if (!bounced && hitFlags.canReflect()) {
                    face = face.turned(4);
                    func_800A2758(&next, face);
                    bounced = 1;
                    func_80049CB4(sound | 0x1000, u, &start, &next);
                    func_800A665C((UnitView_800A7204 *)hit, &face);
                    func_80049CB4(0x1066, hit);
                    start = next;
                    cur = next;
                    continue;
                }
                target = (UnitView_800A7204 *)hit;
            } else {
                func_800A2758(&next, face.turned(4));
            }
            break;
        }
        if (func_800B56F0(&next)) {
            func_80049CB4(sound, u, &start, &next);
            u->pos = next;
            func_80049CB4(6);
            s32 name = func_80049CB4(0x116, &next);
            func_80049CB4(7);
            if (!moved) {
                moved = 1;
                FlagsView_800A7204 moveFlags(u->flags);
                if (!moveFlags.isSilent()) {
                    func_80049CB4(6);
                    func_800497F0(0x103, name, func_800A3B20(u));
                    func_80049CB4(7);
                }
            }
            Effect_800A7204 fx(attacker, func_800B5768(&next));
            func_80049CB4(6);
            func_800A7ADC(u, &fx);
            func_80049CB4(7);
            if (u->isGone()) {
                return;
            }
            start = next;
        }
        cur = next;
    }
    func_80049CB4(sound | 0x1000, u, &start, &next);
    u->pos = next;
    if (target) {
        u16 before = 0;
        if (u->status & 0x7C) {
            before = func_800E08B0(u);
        }
        func_800A0600();
        target->state |= 0x100;
        func_800A7B18(target, attacker, 5, 0x26);
        target->state &= ~0x100;
        func_800A0618(u);
        u16 after = (u->status & 0x7C) ? func_800E08B0(u) : 0;
        if (after < before) {
            damage = 0;
        }
        if (u->isGone()) {
            return;
        }
    }
    if (func_800A4314(u, &next)) {
        func_800A58FC(u, &next);
    } else {
        s32 i;
        MapPos step;
        face = face.turned(4);
        start = next;
        for (i = 7; i != -1; i--) {
            func_800A2594(&step, &start, face);
            next = step;
            if (func_800A4314(u, &next)) {
                func_80049CB4(0x8C, u, &start, &next);
                func_800A58FC(u, &next);
                break;
            }
            func_800A2F94(&face, 1);
        }
        if (i < 0) {
            if (!func_800A60D8(u, &start)) {
                u->vanish();
                return;
            }
            func_800A5168(u, &start, 0);
        }
    }
    if (damage) {
        func_800A7B18(u, attacker, damage, kind);
        if (u->isGone()) {
            return;
        }
    }
    s32 nowKind = u->kind & 0xF;
    next = u->pos;
    if (origKind != nowKind && nowKind == 1 && (func_800B1C6C(&next) & 0x2000)) {
        func_80049CB4(0x10B, &next);
    }
    func_80049CB4(0x1F, u);
    if (notify) {
        func_800A7BA4(u, 1);
    }
}
