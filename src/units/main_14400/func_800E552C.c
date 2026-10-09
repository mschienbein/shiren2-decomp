#include "common.h"
typedef struct { s32 x, y; } Point;
typedef struct { unsigned char value; } Direction;
typedef struct { u32 bits; } Flags;
typedef struct Unit {
    Point position; Direction direction; unsigned char pad09[0x15]; unsigned char typeFlags;
    unsigned char field1F; Flags flags;
} Unit;
typedef struct { Unit *source; s32 kind; unsigned char payload08[6]; unsigned short flags; } Damage;
typedef struct { unsigned short flags, amount; } Outcome;
typedef struct Rng Rng;
extern Rng D_80147620;
extern unsigned short D_80156A42;
extern unsigned short func_800E08B0(void *unit);
extern s32 func_800E547C(Unit *unit, Unit *source);
extern void func_800E20F0(Unit *unit);
extern s32 func_800A6FD0(Unit *unit);
extern s32 func_80049CB4(s32 command, ...);
extern char *func_800A3B20(Unit *unit);
extern void func_800E3678(Unit *unit, Unit *source);
extern s32 func_800E2044(Unit *unit);
extern void func_800497F0(s32 message, ...);
extern void *func_800A2594(Point *out, void *origin, Direction direction);
extern void *func_800B4928(Point *point);
extern s32 func_800A44F4(void *unit, void *candidate);
extern s32 func_800C5844(void *rng, unsigned char minimum, unsigned char maximum);
extern s32 func_800A692C(Unit *unit, s32 status);
extern void *func_800A65E4(Direction *out, Unit *unit, void *target);
extern void func_800A7204(Unit *target, Unit *source, Direction *direction, s32 a, s32 b, s32 c, s32 d, s32 e);
extern void func_800A7ADC(Unit *unit, Damage *damage);
static inline Point *copyPoint(Point *out, Point *point) {
    out->x = point->x;
    out->y = point->y;
    return out;
}
static inline Flags *getFlags(Flags *flags, Unit *unit) {
    flags->bits = unit->flags.bits;
    return flags;
}
static inline Direction *getDirection(Direction *direction, Unit *unit) {
    direction->value = unit->direction.value;
    return direction;
}
void func_800E552C(Unit *unit, Damage *damage, Outcome *outcome) {
    Point point;
    Unit *candidates[8];
    Flags savedFlags;
    Direction facing, redirected, incoming, direction;
    s32 hidden, allowed, context;
    Unit *source;
    Unit *target;
    if (func_800E08B0(unit) == 0) { outcome->flags |= 2; return; }
    if (damage->flags & 1) allowed = 1;
    else if ((damage->flags >> 1) & 1) allowed = 0;
    else allowed = func_800E547C(unit, damage->source);
    source = damage->source;
    func_800E20F0(unit);
    hidden = 0;
    getFlags(&savedFlags, unit);
    if (source && func_800A6FD0(source)) hidden = 1;
    else if (func_800A6FD0(unit)) hidden = 1;
    context = hidden ? -1 : func_80049CB4(0xDA, copyPoint(&point, &unit->position));
    if (!allowed) {
        outcome->flags |= 8;
        func_80049CB4(0x15, unit);
        if (source) {
            char *name = func_800A3B20(source);
            func_800E3678(unit, source);
            if (func_800A6FD0(source)) {
                func_800497F0(0x36, context, name);
            } else if (func_800A6FD0(unit)) {
                s32 other = func_800E2044(unit) ^ 1;
                if (other) func_800497F0(0x36, context, name);
                else func_800497F0(0x37, context, name);
            } else {
                func_800497F0(0x38, context, func_800A3B20(unit), name);
            }
            return;
        }
        func_800497F0(0x39, context, func_800A3B20(unit));
        return;
    }
    if (damage->flags & 0x180) {
        s32 message = 0x3B;
        if (damage->flags & 0x80) {
            message = 0x3A;
            if (damage->source && ((damage->source->typeFlags >> 4) & 1)) message = 0x125;
        }
        func_800497F0(message, context);
    }
    target = unit;
    if (((savedFlags.bits >> 19) & 1) && source && damage->kind == 1) {
        s32 count = 0;
        s32 remaining = 8;
        Unit **next = candidates;
        getDirection(&facing, target);
        for (;;) {
            Unit *candidate;
            s32 acceptable;
            Direction step;
            if (--remaining == -1) break;
            step.value = (facing.value - remaining) & 7;
            func_800A2594(&point, unit, step);
            candidate = func_800B4928(&point);
            acceptable = 0;
            if (candidate && candidate != damage->source && (candidate->typeFlags & 0x7C)) acceptable = func_800A44F4(unit, candidate) == 2;
            if (acceptable) { *next++ = candidate; count++; }
        }
        if (count > 0) {
            char *name;
            target = candidates[(unsigned char)func_800C5844(&D_80147620, 0, (unsigned char)(count - 1))];
            name = func_800A3B20(unit);
            func_800497F0(0x101, context, name, func_800A3B20(source));
            outcome->flags |= 0x10;
        }
    }
    if (func_800A692C(target, 0xE) && damage->kind == 1) {
        if ((outcome->flags >> 4) & 1) {
            func_800A65E4(&redirected, unit, target);
            direction = redirected;
        } else {
            func_800A65E4(&incoming, unit, source);
            direction.value = (incoming.value + 4) & 7;
        }
        func_800A7204(target, source, &direction, D_80156A42, 0, 6, 0, 1);
    }
    outcome->amount = func_800E08B0(target);
    func_800A7ADC(target, damage);
    outcome->amount -= func_800E08B0(target);
    if (func_800E08B0(target) == 0) outcome->flags |= 1;
}
