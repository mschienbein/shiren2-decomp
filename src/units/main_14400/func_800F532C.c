#include "common.h"
typedef struct { s32 x, y; } Point;
typedef struct { u32 bits; } Flags;
typedef struct { s32 kind; void *source; unsigned char payload[8]; s32 value; } Message;
typedef struct { unsigned char pad00[0x58]; short adjustment; unsigned short pad5A; s32 (*message)(void *, Message *); } Methods;
typedef struct Unit {
    Point position; unsigned char direction, terrain; unsigned char pad0A[0x12];
    unsigned short status, field1E; Flags flags; Methods *methods;
} Unit;
typedef struct { s32 index; Unit *source; Point position; s32 bounds[4]; } Iterator;
typedef struct Rng Rng;
extern Rng D_80147620;
extern unsigned char D_80156A3D;
extern s32 func_800C587C(void *rng, unsigned char limit);
extern s32 func_80049CB4(s32 id, ...);
extern void func_800497F0(s32 message, ...);
extern char *func_800A3B20(Unit *unit);
extern Iterator *func_800A915C(Iterator *iterator, Unit *unit);
extern s32 func_800A9284(Iterator *iterator, s32 filter);
extern Unit *func_800A942C(Iterator *iterator);
extern u32 func_800B1C6C(Point *position);
extern s32 func_800A6E90(void *unit);
extern s32 func_800A692C(Unit *unit, s32 status);
static inline Point *copyPoint(Point *to, const Point *from) {
    to->x = from->x;
    to->y = from->y;
    return to;
}
/* ODD_C: tile lookup helper; passing the copy's address through it keeps each call's
   argument a fresh frame address instead of a loop-hoisted pointer. */
static inline u32 tileFlags(Point *position) {
    return func_800B1C6C(position);
}
/* Tests a flag mask on a snapshot of the unit's status word. */
static inline s32 flagsHas(const Flags *flags, u32 mask) {
    return (flags->bits & mask) != 0;
}
void func_800F532C(Unit *unit) {
    Point original;
    Message message;
    Iterator iterator;
    if ((func_800C587C(&D_80147620, D_80156A3D) ^ 1) == 0) {
        if (func_80049CB4(0xDA, copyPoint(&original, &unit->position)) != -2) {
            s32 context;
            func_80049CB4(0x1131);
            context = func_80049CB4(0x10B3, unit);
            func_80049CB4(6);
            func_800497F0(0x115, context, func_800A3B20(unit));
            func_80049CB4(7);
            func_80049CB4(0x132);
        }
        message.kind = 11;
        message.source = 0;
        message.value = 0;
        func_800A915C(&iterator, unit);
        while (func_800A9284(&iterator, 0x7C)) {
            Unit *target = func_800A942C(&iterator);
            Point position;
            s32 blocked = 0;
            copyPoint(&position, &target->position);
            if ((tileFlags(&position) & 0x4000) || func_800A6E90(target) ||
                ((tileFlags(&position) & 0x2000) && (target->terrain & 15) == 2) ||
                ((target->status & 0x40) && !func_800A692C(target, 0x14))) {
                blocked = 1;
            } else {
                Flags flags = target->flags;
                if (flagsHas(&flags, 0x800000)) blocked = 1;
            }
            if (!blocked) {
                func_80049CB4(6);
                target->methods->message((unsigned char *)target + target->methods->adjustment, &message);
                func_80049CB4(7);
            }
        }
    }
}
