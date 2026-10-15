#include "common.h"
#include "map_types.h"
#include "map_line_iterator.h"
extern "C" {
#include "item_records.h"
}

/*
 * Projectile hit/step handler (C++ TU): the first slot of vtable D_80153F70
 * after the null header. action is the movement step kind, direction the
 * by-value one-byte MapDir (homed at sp+0x7F). Messages to items and units are
 * Event_800C37B8 objects; every event is a block-scoped local or a const-reference
 * temporary, so all of them share sp+0x18.. and the frame is 0x70 as in ROM.
 * The three one-byte direction temporaries at sp+0x48..0x4A are the
 * const-reference bindings of turn(), func_800E370C and func_800C25D0.
 * Virtual calls use the g++ {delta, index, pfn} slots.
 *
 * The pierce loop reuses the function's hit and name locals for each listed
 * unit and its message name (one pseudo each, as in ROM: hit s0, self s1,
 * name s2). The two hit reports are Event subclasses: PierceHitEvent sets
 * its type in its own body; HitEvent is built from the projectile, crediting
 * the reflecting unit if there is one (Projectile::attacker()).
 */

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

static inline MapDir opposite(MapDir d)
{
    MapDir r;
    r.value = (d.value + 4) & 7;
    return r;
}

/* This function's view of the 0x20-byte event message: item message header at +0, two
   argument words, MapDir +0xC, MapPos +0x10, value +0x18, source +0x1C. */
struct Event_800C37B8 {
    /* All callers construct nonconst events; const-reference helpers also bind
     * temporary reports, while item dispatch accepts a mutable message header. */
    mutable ItemMessage base;
    void *arg4;
    void *arg8;
    MapDir direction;
    MapPos pos;
    s32 value18;
    void *source1C;
    Event_800C37B8(s32 t, void *a, void *b) { base.kind = t; arg4 = a; arg8 = b; }
    Event_800C37B8() {}
    Event_800C37B8(s32 t, void *a, void *b, s32 v, const MapDir &d, void *src)
    {
        base.kind = t;
        arg4 = a;
        arg8 = b;
        direction = d;
        value18 = v;
        source1C = src;
    }
    Event_800C37B8(s32 t, void *a, void *b, const MapPos &p, const MapDir &d)
    {
        base.kind = t;
        arg4 = a;
        arg8 = b;
        pos = p;
        direction = d;
    }
    Event_800C37B8(s32 t, const MapPos &p, const MapDir &d)
    {
        base.kind = t;
        pos = p;
        direction = d;
    }
    Event_800C37B8(s32 t, void *a, const MapPos &p) { base.kind = t; arg4 = a; pos = p; }
};


struct PierceHitEvent_800C37B8 : Event_800C37B8 {
    PierceHitEvent_800C37B8(void *a, void *b, s32 v, const MapDir &d, void *src)
    {
        base.kind = 0x13;
        arg4 = a;
        arg8 = b;
        direction = d;
        value18 = v;
        source1C = src;
    }
};

struct Slot_800C37B8 { s16 delta, index; void *pfn; };
struct TestSlot_800C37B8 { s16 delta, index; s32 (*fn)(void *self); };
struct NotifySlot_800C37B8 { s16 delta, index; s32 (*fn)(void *self, const Event_800C37B8 *event); };


/* Partial views of the unit vtable (+0x10 query, +0x58 notify entries) and of the unit record.
   Common prefix with the other b7 unit views: MapPos at +0, the +0x1E flag byte and the +0x24
   vtable (UnitView_800A7204, UnitView_800C62B8). */
struct UnitVTableView_800C37B8 { Slot_800C37B8 slot0[2]; TestSlot_800C37B8 query; Slot_800C37B8 slot3[8]; NotifySlot_800C37B8 notify; };
struct UnitView_800C37B8 {
    MapPos pos;
    u8 pad08[4];
    MapRect area;
    u8 pad1C[2];
    u8 flags1E;
    u8 pad1F[5];
    UnitVTableView_800C37B8 *vt;
    u8 pad28[0x30];
    s32 value58;
    u8 pad5C[8];
    MapPos target64;
};

struct Projectile_800C37B8 {
    MapPos pos;
    MapDir direction;
    u8 pad09[0xF];
    u8 active18;
    u8 pad19[7];
    UnitView_800C37B8 *owner20;
    Item *item24;
    s32 value28;
    MapPos origin2C;
    u16 flags34;
    u8 pad36[2];
    UnitView_800C37B8 *reflected38;
    u8 hit_count3C;
    u8 pad3D[3];
    UnitView_800C37B8 *hits40[30];
    s32 specialB8;
    u8 point_countBC;
    u8 padBD[3];
    MapPos pointsC0[10];
    MapDir directions110[10];

    UnitView_800C37B8 *attacker() const { return reflected38 ? reflected38 : owner20; }
};

struct HitEvent_800C37B8 : Event_800C37B8 {
    HitEvent_800C37B8(Projectile_800C37B8 *p, void *b, s32 v, const MapDir &d)
        : Event_800C37B8(0x12, p->attacker(), b, v, d, p->owner20)
    {
    }
};

extern "C" {
extern u8 D_8014344C;
char *func_800AC9D8(Item *item);
char *func_800AC990(void *item);
char *func_800A3B20(UnitView_800C37B8 *unit);
UnitView_800C37B8 *func_800B4928(MapPos *pos);
Item *func_800B4D80(MapPos *pos);
s32 func_800A6FD0(UnitView_800C37B8 *unit);
s32 func_80049CB4(s32 id, ...);
s32 func_8005DFE8(u8 *str);
s32 func_800A5440(void *object, void *target, u8 *direction, s32 kind, char *item);
s32 func_800A31C8(MapRect *area, MapPos *point);
u32 func_800B1C6C(MapPos *pos);
void *func_800A2594(MapPos *out, MapPos *from, MapDir dir);
MapPos *func_800A25D8(MapPos *out, MapPos *from, MapDir dir, s32 count);
void func_800A2758(MapPos *pos, MapDir dir);
void func_800C3754(void *self, MapPos *first, MapPos *second);
void func_800C33FC(void *self, MapPos *target, MapDir dir);
void func_800C428C(void *self);
s32 func_800C414C(void *self, MapPos *from, MapDir dir);
s32 func_800AE324(Item *item, void *pos);
void func_800D3650(void *item);
void func_800E20F0(void *unit);
void func_800E370C(void *unit, u8 *direction);
void func_800497F0(s32 id, ...);
void func_800498E4(s32 id, ...);
void func_80049AE8(s32 id, ...);
}

static inline s32 item_query(Item *item, s32 what)
{
    return item->field08->test1C((u8 *)item + item->field08->delta18, what);
}

static inline void item_notify(Item *item, const Event_800C37B8 &event)
{
    item->field08->handle3C((u8 *)item + item->field08->delta38, &event.base);
}

static inline s32 unit_notify(UnitView_800C37B8 *unit, const Event_800C37B8 &event)
{
    return unit->vt->notify.fn((u8 *)unit + unit->vt->notify.delta, &event);
}

static inline void item_destroy(Item *item)
{
    if (item) {
        item->field08->method0C((u8 *)item + item->field08->delta08, 3);
    }
}

static inline void turn(Projectile_800C37B8 *self, const MapDir &d)
{
    self->direction = d;
}

static inline void remember_point(Projectile_800C37B8 *self, const MapPos *p, const MapDir &d)
{
    if (self->point_countBC < 10) {
        self->pointsC0[self->point_countBC] = *p;
        u8 i = self->point_countBC++;
        self->directions110[i] = d;
    }
}

extern "C" s32 func_800C37B8(Projectile_800C37B8 *self, s32 action, MapPos *position, MapDir direction)
{
    char *name = func_800AC9D8(self->item24);
    UnitView_800C37B8 *hit = func_800B4928(position);
    s32 message;

    if (self->owner20 && func_800A6FD0(self->owner20)) {
        message = -1;
    } else {
        message = func_80049CB4(0xDA, position);
    }
    message = message == -2 ? func_80049CB4(0xDA, &self->origin2C) : message;
    if (self->flags34 & 0x20) {
        if (action == 0) {
            MapPos end;
            func_800A25D8(&end, position, direction, 5);
            func_800C3754(self, &self->origin2C, &end);
            for (s32 i = 0; i < self->hit_count3C; i++) {
                hit = self->hits40[i];
                if (hit->vt->query.fn((u8 *)hit + hit->vt->query.delta)) {
                    continue;
                }
                if (!func_800A5440(hit, self->owner20, &direction.value, self->flags34,
                                   func_800AC9D8(self->item24))) {
                    name = func_800AC9D8(self->item24);
                    if (func_800A6FD0(hit)) {
                        message = -1;
                    }
                    if (func_8005DFE8((u8 *)name) < 0x34) {
                        func_800497F0(0x63, message, name, func_800A3B20(hit));
                    } else {
                        func_800497F0(0x64, message, name, func_800A3B20(hit));
                    }
                    PierceHitEvent_800C37B8 event(self->owner20, hit, (self->flags34 >> 5) & 1, direction, self->owner20);
                    item_notify(self->item24, event);
                }
                if (hit->flags1E & 0x7C) {
                    func_800E20F0(hit);
                }
            }
            if ((item_query(self->item24, 0x21) ^ 1) != 0) {
                func_800497F0(0x87, message, func_800AC9D8(self->item24));
            }
            func_800C428C(self);
            func_800D3650(self->item24);
            item_destroy(self->item24);
            return 0;
        }
        if (action == 4) {
            u8 i = self->hit_count3C++;
            self->hits40[i] = hit;
        } else if (action == 5 && self->specialB8) {
            Item *target = func_800B4D80(position);
            if (target && target->field00 == 0x10) {
                remember_point(self, position, direction);
            }
        }
        return 1;
    }
    switch (action) {
    case 0: {
        if (func_800B1C6C(position) & 0x4000) {
            return 1;
        }
        MapPos end;
        func_800A25D8(&end, position, direction, 5);
        func_800C3754(self, &self->origin2C, &end);
        if (self->owner20 && (self->owner20->flags1E & 0xC)) {
            func_800498E4(0x87, func_800AC990(self->item24));
        }
        func_800C428C(self);
        func_800D3650(self->item24);
        item_destroy(self->item24);
        return 0;
    }
    case 1:
    case 6: {
        MapPos end(self->origin2C);
        if (action == 1 && func_800C414C(self, position, direction)) {
            MapPos turned;
            func_800A2594(&turned, position, opposite(direction));
            func_800C3754(self, &end, &turned);
            self->active18 = action;
            return 1;
        }
        func_800C3754(self, &self->origin2C, position);
        func_800C33FC(self, position, direction);
        func_800C428C(self);
        return 0;
    }
    case 3:
        if (item_query(self->item24, 0x24)) {
            return 1;
        }
        func_800C3754(self, &self->origin2C, position);
        func_800AE324(self->item24, position);
        func_800C428C(self);
        return 0;
    case 5:
        if (self->specialB8) {
            Item *target = func_800B4D80(position);
            if (target->field00 == 0x10) {
                switch (target->field01) {
                case 0xDB:
                    func_800C3754(self, &self->origin2C, position);
                    item_notify(target, Event_800C37B8(0x15, *position, direction));
                    item_notify(self->item24, Event_800C37B8(0xE, self->owner20, *position));
                    func_800C428C(self);
                    return 0;
                case 0xD5:
                case 0xD6:
                    func_800C3754(self, &self->origin2C, position);
                    item_notify(target, Event_800C37B8(0x17, 0, self->item24, *position, direction));
                    func_800C428C(self);
                    return 0;
                }
                remember_point(self, position, direction);
            }
        }
        return 1;
    case 4: {
        func_800C3754(self, &self->origin2C, position);
        if (!self->reflected38 && !(self->flags34 & 2) &&
            unit_notify(hit, Event_800C37B8(0xF, self->owner20, self->item24))) {
            func_800C428C(self);
            return 0;
        }
        s32 result;
        switch (func_800A5440(hit, self->owner20, &direction.value, self->flags34, name)) {
        case 1:
            func_800C33FC(self, &hit->pos, direction);
            func_800C428C(self);
            result = 0;
            break;
        case 2:
            turn(self, opposite(direction));
            func_800A2758(position, opposite(direction));
            self->pos = *position;
            self->active18 = 1;
            self->origin2C = *position;
            self->value28 = 0;
            self->reflected38 = hit;
            self->flags34 |= 0x10;
            result = 1;
            break;
        default: {
            if (self->item24->field01 != 0xAC) {
                if (func_800A6FD0(hit)) {
                    message = -1;
                }
                if (func_8005DFE8((u8 *)name) < 0x34) {
                    func_80049AE8(0x63, message, name, func_800A3B20(hit));
                } else {
                    func_80049AE8(0x64, message, name, func_800A3B20(hit));
                }
            }
            HitEvent_800C37B8 event(self, hit, 0, direction);
            item_notify(self->item24, event);
            func_800C428C(self);
            result = 0;
            break;
        }
        }
        if (hit->flags1E & 0x7C) {
            func_800E20F0(hit);
            func_80049CB4(0x132);
            MapDir reverse = opposite(direction);
            func_800E370C(hit, &reverse.value);
            if ((hit->flags1E >> 4) & 1) {
                s32 trace = 0;
                if (D_8014344C >= 2 && (func_800B1C6C(position) & 0x1000)) {
                    trace = hit->value58 == 0;
                }
                if (trace) {
                    MapRect area(hit->area);
                    MapLineIterator ray;
                    MapDir backwards = opposite(direction);
                    func_800C25D0(&ray, position, &backwards.value, 0x7F);
                    while (ray.index < ray.length) {
                        MapPos p;
                        func_800C2758(&p, &ray);
                        if (func_800B1C6C(&p) & 0x800) {
                            hit->target64 = p;
                            break;
                        }
                        if ((func_800A31C8(&area, &p) ^ 1) != 0) {
                            break;
                        }
                    }
                }
            }
        }
        return result;
    }
    case 7:
        if (self->specialB8 && (func_800B1C6C(position) & 0x2100)) {
            func_800C3754(self, &self->origin2C, position);
            item_notify(self->item24, Event_800C37B8(0xE, self->owner20, *position));
            func_800C428C(self);
            return 0;
        }
        return 1;
    default:
        return 1;
    }
}
