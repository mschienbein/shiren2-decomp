#include "common.h"

typedef unsigned char u8;
typedef struct Position { s32 x, y; } Position;
typedef struct Dir { u8 value; } Dir;
typedef struct Unit {
    Position position;
    Dir direction_08;
    u8 pad_09[0x15];
    u8 flags_1E;
    u8 pad_1F[0xC5];
    unsigned short flags_E4;
} Unit;
typedef struct Rng Rng;
extern Rng D_80147620;
extern Unit *D_801476B8;
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
/* Direction offsets probed in preference order (front first, back last). */
extern const s32 D_80158334[];
extern Unit *func_800C5F60(void);
extern s32 func_800E1CC4(Unit *object, s32 kind);
extern s32 func_800C5844(void *rng, u8 base, u8 top);
extern void func_800A2F80(u8 *direction, s32 offset);
extern void *func_800A2594(Position *out, void *position, Dir direction);
extern void *func_800B4928(Position *position);
extern s32 func_800A4520(void *actor, Unit *target);
extern u8 func_800A6420(Unit *actor, Unit *target);
extern void func_800A665C(Unit *actor, u8 *direction);

static inline Dir turned(Dir *direction, s32 offset)
{
    Dir result;
    result.value = (direction->value + offset) & 7;
    return result;
}

/* Command run slot (+0x14 of D_80158358): turn toward the most attractive neighbour. */
s32 func_800DA670(void *self /* receiver: unused; supplied by the command table run slot */)
{
    Position origin, adjacent;
    Dir direction, selected;
    Unit *actor = func_800C5F60();
    direction = actor->direction_08;
    if (func_800E1CC4(actor, 0)) {
        func_800A2F80(&direction.value, (u8)func_800C5844(&D_80147620, 1, 7));
        func_800A665C(actor, &direction.value);
        return 1;
    } else {
        s32 highest = 0;
        s32 best = 7;
        s32 i;
        origin.x = actor->position.x;
        origin.y = actor->position.y;
        for (i = 0; ; ++i) {
            Dir offset;
            Unit *target;
            s32 score;
            s32 prefer;
            if (i >= 8)
                break;
            offset = turned(&direction, D_80158334[i]);
            func_800A2594(&adjacent, &origin, offset);
            target = func_800B4928(&adjacent);
            if (!target)
                continue;
            if ((target->flags_1E & 0x7C) && func_800E1CC4(target, 1)) {
                s32 hidden = ((D_801476B8->flags_E4 >> 3) & 1) ^ 1;
                if (hidden)
                    continue;
            }
            score = func_800A4520(actor, target) ? 20 : 1;
            prefer = ((D_80142F18.flags >> 2) & 1) ^ 1;
            if (prefer) {
                switch ((u8)func_800A6420(actor, target)) {
                case 0: score += 10; break;
                case 1: score += 5; break;
                }
            }
            if (score > highest) {
                highest = score;
                best = i;
            }
        }
        selected = turned(&direction, D_80158334[best]);
        func_800A665C(actor, &selected.value);
        return 1;
    }
}
