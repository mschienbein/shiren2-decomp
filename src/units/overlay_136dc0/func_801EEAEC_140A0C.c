#include "func_801EEAEC_140A0C/types.h"
extern S10Actor *D_801476B8;
extern void *func_800A8CB0(s32 id);
extern s32 func_800A8974(void *actor);
extern s32 func_80049CB4(s32 event, ...);
extern s32 func_800A5D2C(void *actor, S10Position *position, s32 flags);
extern void func_800A58FC(void *actor, S10Position *position);

/* ODD_C: predicate and memberwise copy preserve the inlined-method shape.
 * Direct tests/member assignments produce 232 bytes; aggregate copy produces
 * 244 bytes and hoists its stack address. ROM has xori and a local copy base. */
static inline s32 lacks_movement_flag(S10Actor *actor)
{
    u8 flag = (actor->flags1E >> 4) & 1;
    return flag ^ 1;
}

static inline void copy_position(S10Position *out, const S10Position *in)
{
    out->x = in->x;
    out->y = in->y;
}

void func_801EEAEC_140A0C(void)
{
    s32 i = 30;
    for (;;) {
        S10Actor *actor;
        S10Position position;
        if (--i == -1) {
            break;
        }
        actor = func_800A8CB0((u8)i);
        if (func_800A8974(actor)) {
            continue;
        }
        if (actor->vtable24->call14((u8 *)actor + actor->vtable24->delta10)) {
            continue;
        }
        if (lacks_movement_flag(actor)) {
            continue;
        }
        if (!(((S10ActorC *)actor)->flags9A & 0x40)) {
            continue;
        }
        copy_position(&position, &D_801476B8->position);
        if (func_800A5D2C(actor, &position, 10)) {
            func_800A58FC(actor, &position);
        }
    }
    func_80049CB4(2);
}
