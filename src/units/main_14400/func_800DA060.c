#include "common.h"

typedef unsigned char u8;
typedef short s16;

/* 0x18-byte event: type, target, unused tail. */
typedef struct { s32 type; void *target; s32 extra[4]; } Event;
/* Entity method table at +0x24: +0x58/+0x5C dispatch s32 (void *self, void *event). */
typedef struct {
    u8 pad0[0x58];
    s16 delta_58;
    s16 index_5A;
    s32 (*dispatch_5C)(void *self, void *event);
} EntityVTable;
typedef struct { u8 pad : 2; u8 flag5 : 1; u8 flag4 : 1; u8 flag3 : 1; u8 low : 3; } Bits;
typedef struct {
    u8 pad0[0x1E];
    Bits flags;
    u8 pad1F[0x5];
    EntityVTable *vtable;
} Unit;
typedef struct { u8 pad0[0x104]; Unit *unit; } World;

extern World *D_801476B8;
extern u8 D_80147620[];
extern s32 func_800E1D14(Unit *unit, s32 kind);
extern u8 func_800C57CC(void *rng, s32 range);
extern void func_800A665C(Unit *unit, u8 *dir);
extern void *func_800A6CF0(Unit *unit);

static inline s32 isFlag3(Bits b) { return b.flag3; }
static inline s32 isFlag4(Bits b) { return b.flag4; }
static inline s32 isFlag5(Bits b) { return b.flag5; }

/* Command +0x14 run: turn the player's flagged partner to a random direction, then notify it. */
s32 func_800DA060(void *self /* receiver: unused; supplied by the command vtable +0x14 call */) {
    Event event;
    u8 dir;
    Unit *unit = D_801476B8->unit;
    Unit *partner = unit; /* second handle used for the notification */
    Bits bits;

    if (unit != 0 && (bits = unit->flags, isFlag3(bits) | isFlag4(bits) | isFlag5(bits))) {
        Event *ev;
        void *target;

        if (func_800E1D14(unit, 0x13) != 0) {
            dir = func_800C57CC(D_80147620, 7) & 7;
            func_800A665C(unit, &dir);
        }
        ev = &event;
        target = func_800A6CF0(partner);
        event.type = 0x19;
        ev->target = target;
        partner->vtable->dispatch_5C((u8 *)partner + partner->vtable->delta_58, ev);
    }
    return 0;
}

