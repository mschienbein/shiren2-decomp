#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct { s32 x; s32 y; } Pos;
typedef struct { u8 value; } Dir;

/* Message-handler request (0x18 bytes) sent through the unit table +0x5C slot. */
typedef struct {
    s32 type;
    void *target;
    s32 args[4];
} Event;

/* g++ vtable of units (table pointer at unit+0x24): entries are {delta, index, pfn}. */
typedef struct {
    u8 pad00[8];
    s16 destroyDelta;
    s16 destroyIndex;
    void (*destroy)(void *self, s32 flags);
    u8 pad10[0x58 - 0x10];
    s16 handleDelta;
    s16 handleIndex;
    s32 (*handle)(void *self, Event *event);
} UnitVTable;

typedef struct {
    Pos pos;
    Dir dir;
    u8 pad09[0x24 - 0x09];
    UnitVTable *vtable;
    u8 pad28[0x58 - 0x28];
    void *target;
    u8 pad5C[0x74 - 0x5C];
    u8 actions;
    u8 pad75[0x9A - 0x75];
    u16 flags;
} Unit;

typedef struct {
    u8 pad00[8];
    s16 destroyDelta;
    s16 destroyIndex;
    void (*destroy)(void *self, s32 flags);
} SpawnVTable;

/* Spawn descriptor that owns this handler (vtable at +8, flags at +0xC). */
typedef struct {
    u8 pad00[8];
    SpawnVTable *vtable;
    u8 flags;
} Spawn;

typedef struct {
    u8 pad00[0x1E];
    u8 flags1E;
} Actor;

typedef struct {
    s32 type;
    Actor *actor;
    void *item;
    u8 pad0C[4];
    Pos pos;
    s32 dir;
} Msg;

extern Unit *func_80128404(Spawn *spawn);
extern s32 func_800AF28C(Spawn *self, Msg *msg);
extern void *func_800A2594(void *out, void *from, Dir dir);
extern s32 func_800A5D2C(void *object, Pos *output, s32 flags);
extern s32 func_800A4314(Unit *object, Pos *position);
extern s32 func_80049CB4(s32 id, ...);
extern void func_800A665C(Unit *obj, u8 *value);
extern void *func_800A27A4(void *out_direction, void *from, void *to);
extern void func_800A58FC(void *actor, Pos *position);
extern void func_800F02B0(Unit *self, void *item);
extern void func_801217B0(void *item, void *unit);
extern void *func_800A6CF0(void *self);
extern s32 func_800A4520(void *ctx, void *obj);
extern s32 func_800A455C(Unit *obj, void *dest, s32 maxSteps);
extern s32 func_800A67DC(Unit *u, void *o, s32 force, s32 apply);
extern void *func_800A65E4(void *out_direction, void *obj, void *target);
extern void func_800A6690(Unit *unit, u8 *dir, s32 arg2);
extern void *func_800F1A58(Unit *a, s32 b, s32 c);
extern u8 func_800A6420(Unit *obj, void *target);
extern void *func_800A6BA4(void *obj, s32 range, s32 ignore_terrain);
extern void *func_800B4928(Pos *pos);
extern void func_800A2F80(unsigned char *self, s32 step);
extern s32 func_800A44F4(void *self, void *target);
extern void func_800E1048(Unit *o);
extern s32 func_800A529C(void *obj, s32 flag);

static inline void dir_set(Dir *dir, s32 value) {
    dir->value = value;
}
static inline void event_set(Event *ev, s32 type, void *target) {
    ev->type = type;
    ev->target = target;
}
static inline void vec2i_copy(Pos *dst, Pos *src) {
    dst->x = src->x;
    dst->y = src->y;
}
s32 func_801285A0(Spawn *self, Msg *msg) {
    Pos target;
    Pos dest;
    Pos cell;
    Event ev;
    Dir dir;
    Dir speed;
    Dir facing;
    Dir face;
    Dir search;
    Unit *unit;
    s32 blocked;
    s32 heading;
    s32 hit;
    if (msg->type == 0x1B) {
        unit = func_80128404(self);
        if (unit != 0) {
            vec2i_copy(&target, &msg->pos);
            heading = msg->dir;
            if (heading != -1) {
                s32 unreachable;
                dir_set(&dir, heading & 7);
                func_800A2594(&cell, &target, dir);
                dest = cell;
                unreachable = func_800A4314(unit, &dest) ^ 1;
                if (unreachable) {
                    unit->vtable->destroy((u8 *)unit + unit->vtable->destroyDelta, 3);
                    return 0;
                }
            } else {
                dest = target;
                func_800A5D2C(unit, &dest, 1);
            }
            self->flags |= 8;
            blocked = func_800A4314(unit, &dest);
            blocked ^= 1;
            func_80049CB4(0x86, unit);
            speed.value = 4;
            func_800A665C(unit, &speed.value);
            vec2i_copy(&cell, &dest);
            func_800A27A4(&facing, &target, &cell);
            func_800A665C(unit, &facing.value);
            func_80049CB4(0x88, unit);
            func_80049CB4(0x109F, unit, &target, &dest);
            if (!blocked) {
                func_800A58FC(unit, &dest);
            }
            if (self->flags & 2) {
                void *item = msg->item;
                Actor *actor;
                func_800F02B0(unit, item);
                func_801217B0(item, unit);
                if (!blocked) {
                    actor = msg->actor;
                    if (actor != 0 && ((actor->flags1E >> 2) & 1)) {
                        void *foe;

                        hit = 0;
                        foe = func_800A6CF0(actor);
                        if (foe != 0 && func_800A4520(unit, foe)) {
                            if (func_800A455C(unit, foe, 1) ||
                                ((unit->flags & 1) && func_800A67DC(unit, foe, hit, 1))) {
                                hit = 1;
                            }
                        }
                        if (hit) {
                            func_800A65E4(&face, unit, foe);
                            func_800A6690(unit, &face.value, 1);
                        } else {
                            foe = 0;
                            if (unit->flags & 2) {
                                s32 bad;
                                foe = func_800F1A58(unit, 0x4C, 1);
                                bad = 0;
                                if (func_800A6420(unit, foe) == 3 || !func_800A4520(unit, foe)) {
                                    bad = 1;
                                }
                                if (bad) {
                                    foe = 0;
                                } else {
                                    unit->target = foe;
                                }
                            } else if (unit->flags & 1) {
                                foe = func_800A6BA4(unit, 0x4C, 0);
                                if (func_800A4520(unit, foe)) {
                                    unit->target = foe;
                                }
                            }
                            if (foe == 0) {
                                s32 i;
                                search = unit->dir;
                                for (i = 0; i < 8; i++) {
                                    s32 ok;
                                    func_800A2594(&cell, &dest, search);
                                    ok = 0;
                                    foe = func_800B4928(&cell);
                                    if (func_800A4520(unit, foe)) {
                                        ok = func_800A455C(unit, foe, 1) != 0;
                                    }
                                    if (ok) {
                                        func_800A6690(unit, &search.value, 1);
                                        break;
                                    }
                                    func_800A2F80(&search.value, 1);
                                }
                                if (i == 8) {
                                    foe = 0;
                                }
                            }
                        }
                        if (func_800A44F4(unit, foe) == 2) {
                            event_set(&ev, 0x19, func_800A6CF0(unit));
                            unit->actions++;
                            unit->vtable->handle((u8 *)unit + unit->vtable->handleDelta, &ev);
                            func_800E1048(unit);
                        }
                    }
                }
            }
            if (blocked) {
                unit->pos = dest;
                func_800A529C(unit, 0);
            }
            if (self != 0) {
                self->vtable->destroy((u8 *)self + self->vtable->destroyDelta, 3);
            }
            return 1;
        }
        return 0;
    }
    return func_800AF28C(self, msg);
}
