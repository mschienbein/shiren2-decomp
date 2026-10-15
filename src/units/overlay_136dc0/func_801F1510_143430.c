#include "func_801F1510_143430/types.h"
extern S10Selection D_80142F18;
extern S10Actor *D_801476B8;
extern void *func_800A8CB0(s32 id);
extern s32 func_800A8974(void *actor);
extern s32 func_80049CB4(s32 event, ...);
extern void *func_800A83D0(s32 kind, u8 flags);
extern S10Actor *func_800D7CF0(u8 kind, u8 level);
extern s32 func_800D8144(s32 index);
extern s32 func_800D818C(s32 value);
extern s32 func_800A6184(void *actor, S10Position *position);
extern s32 func_800A6218(void *actor, S10Position *position, s32 notify);
extern void func_800A58FC(void *actor, S10Position *position);
extern void func_800A665C(void *actor, u8 *direction);
extern s32 func_800B1E80(S10Position *position);
extern void func_800EE718(void *actor, s32 mode);
extern s32 func_801E8D0C_13AC2C(u16 kind, u16 flag);
extern S10Actor *func_801F17C0_1436E0(s32 kind, s32 level);
extern s32 func_801F2E30_144D50(s32 kind);
extern s32 func_801F2E80_144DA0(s32 kind);

static inline S10ActorComponent *component78(S10Actor *actor)
{
    return actor ? &((S10ActorA *)actor)->component78 : 0;
}

/* ODD_C: keep the source-level predicates separate. The direct expressions
 * use compare registers instead of the ROM's xori and eliminate a position
 * reload. The baseline candidate emits 680 bytes rather than 688. */
static inline s32 special_scene(void)
{
    return D_80142F18.mode == 0x4F;
}

static inline s32 position_y(S10Position *position)
{
    return position->y;
}

static inline s32 position_is_clear(S10Position *position)
{
    return func_800B1E80(position) ^ 1;
}

S10Actor *func_801F1510_143430(S10SpawnCommand *command, s32 level, s32 place)
{
    S10Position position;
    u8 direction;
    S10Actor *actor;
    s32 category = 0;
    if (level < 2 && D_80142F18.mode == 0x6A &&
        (u32)(command->kind - 0x18) < 5) {
        level = 5;
    }
    if (func_801F2E30_144D50(command->kind)) {
        actor = func_800A8CB0((u8)func_801E8D0C_13AC2C(command->kind, 0));
    } else {
        s32 special = special_scene() && func_801F2E80_144DA0(command->kind);
        if (special) {
            if (command->kind == 0x29) {
                level = func_800D8144(func_800D818C(level));
            }
            actor = func_800D7CF0(command->kind, (u8)level);
            category = 2;
        } else {
            actor = func_800A83D0(command->kind, (u8)level);
            category = 1;
        }
    }
    if (func_800A8974(actor) && command->kind == 0x5E) {
        actor = func_801F17C0_1436E0(0x5E, level);
    }
    if (func_800A8974(actor)) {
        return 0;
    }
    if (category == 1) {
        if ((actor->flags1E >> 3) & 1) {
            func_800EE718(actor, 2);
        }
    } else if (category == 2) {
        ((S10ActorC *)actor)->flags9A &= 0xFDBF;
    }
    position.y = command->coordinates.bytes.y;
    position.x = command->coordinates.bytes.x;
    if (place) {
        func_800A6218(actor, &position, 0);
    } else if ((position_y(&position) | position.x) == 0) {
        s32 i;
        for (i = 0; ; i++) {
            s32 clear;
            if (i >= 100) {
                break;
            }
            func_800A6184(actor, &D_801476B8->position);
            clear = position_is_clear(&actor->position);
            if (clear) {
                break;
            }
        }
    } else {
        func_800A58FC(actor, &position);
    }
    direction = command->direction & 7;
    func_800A665C(actor, &direction);
    if (actor->kind0A == 0x5F) {
        S10ComponentMethod *method;
        component78(actor)->field18 = 0x297;
        component78(actor)->field00 = 1;
        method = &component78(actor)->vtable30->method08;
        method->call((u8 *)component78(actor) + method->delta, 1);
    }
    func_80049CB4(2);
    return actor;
}
