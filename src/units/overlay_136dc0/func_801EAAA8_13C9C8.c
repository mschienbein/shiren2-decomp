/* func_801EAAA8_13C9C8 (overlay_136dc0): event command "change unit stat" (args 0..3).
 * Unit vtable calls and the null-checked +0x78/+0x84 component conversions follow
 * the canonical C model of g++ code (func_800DF61C, func_800B7680).
 *
 * ODD_C: target/unit/actor are three copies of one pointer. ROM keeps target in s3,
 * unit in s0 and actor in s1 (move s0,s3 and move s1,s0 in the delay slots). One shared
 * pointer measured 336 words (text 1376).
 * ODD_C: the 0x78 path repeats the null-checked conversion for the vtable load and for
 * `this` (canonical func_800DF61C shape). Plain member access there measured 105 words
 * (text 1336).
 * ODD_C: the 0x84 path keeps `comp` block-local and assigns it after the vtable load.
 * The reviewed sched1 dump shows a liveness/single-set priority boost for this lifetime;
 * single assignment alone does not guarantee that scheduling. Both adjustable byte
 * receivers derive from the enclosing Unit, so a negative delta may reach its base.
 * Measured alternatives:
 *   - comp shared with the 0x78 path: 102 words, text 1412;
 *   - comp initialized at its declaration: 79 words, text 1408;
 *   - comp->vtable used directly: 102 words, text 1412;
 *   - store after the vtable load: 107 words, text 1408;
 *   - store last: 65 words.
 */
#include "func_801EAAA8_13C9C8/types.h"

extern s32 func_801E8D0C_13AC2C(u16 id, u16 selector);
extern void func_801E6A8C_1389AC(u8 cell);
extern Unit *func_800A8CB0(s32 cell);
extern s32 func_800A8974(void *obj);
extern s32 func_800EB37C(Unit *unit);
extern s32 func_800EB3A4(Unit *unit);
extern void func_800EB330(Unit *unit, u8 value);
extern void func_800EB35C(Unit *unit, u8 value);
extern u16 func_800E08B0(Unit *unit);
extern u16 func_800E08F0(Unit *unit);
extern s32 func_800E0534(Unit *unit, s32 amount);
extern s32 func_800E07A8(Unit *unit, s32 amount);
extern s32 func_800E0AB4(Unit *unit, s32 amount);
extern s16 func_800E0BD0(Unit *unit, s32 amount);
extern s32 func_800E0F40(Unit *unit);
extern s32 func_80049CB4(s32 action, ...);
extern void func_800485DC(void *obj);
extern void *D_801476B8;

void func_801EAAA8_13C9C8(void) {
    OvlState *st = D_801F51E8_147108;
    u16 id = st->field108[0].word;
    s32 mode = st->field108[1].word;
    s32 kind = st->field108[2].word;
    s32 value = st->field108[3].word;
    Unit *target = func_800A8CB0((u8)func_801E8D0C_13AC2C(id, 0));
    Unit *unit;
    Unit *actor;
    s32 cur;
    s32 amount;
    ComponentSlot *slot;
    ComponentVtbl *vt;
    u8 *c;

    if (func_800A8974(target) != 0) {
        return;
    }
    unit = target;
    actor = unit;
    switch (kind) {
    case 0x1E:
        cur = (u8)func_800EB37C(target);
        amount = (mode != 0) ? value + cur : value;
        if ((u8)func_800EB3A4(target) < amount) {
            amount = (u8)func_800EB3A4(target);
        }
        func_800EB330(target, amount);
        break;
    case 0x1F:
        amount = (mode != 0) ? value + (u8)func_800EB3A4(target) : value;
        func_800EB35C(target, amount);
        break;
    case 0x20:
        cur = func_800E08B0(actor);
        amount = (mode == 0) ? value - cur : value;
        if (func_800E08F0(actor) < amount + cur) {
            amount = func_800E08F0(actor) - cur;
        }
        func_800E0534(actor, amount);
        break;
    case 0x21:
        if (mode == 0) {
            func_800E07A8(actor, value - func_800E08F0(actor));
        } else {
            func_800E07A8(actor, value);
        }
        break;
    case 0x22:
        cur = (u16)actor->vtable->fn68((u8 *)actor + actor->vtable->delta68);
        amount = (mode == 0) ? value - cur : value;
        if ((u16)actor->vtable->fn70((u8 *)actor + actor->vtable->delta70) < amount + cur) {
            amount = (u16)actor->vtable->fn70((u8 *)actor + actor->vtable->delta70) - cur;
        }
        func_800E0AB4(actor, amount);
        break;
    case 0x23:
        if (mode == 0) {
            func_800E0BD0(actor, value - (u16)actor->vtable->fn70((u8 *)actor + actor->vtable->delta70));
        } else {
            func_800E0BD0(actor, value);
        }
        break;
    case 0x24:
        if (mode == 0) {
            amount = value - (u8)func_800E0F40(actor);
            actor->vtable->fn78((u8 *)actor + actor->vtable->delta78, amount);
        } else {
            actor->vtable->fn78((u8 *)actor + actor->vtable->delta78, value);
        }
        break;
    default:
        func_801E6A8C_1389AC((u8)func_801E8D0C_13AC2C(id, 0));
        if (mode == 1) {
            switch (kind) {
            case 0x18:
                unit->vtable->fn90((u8 *)unit + unit->vtable->delta90, 1, 0x11, 0, 0);
                unit->vtable->fn90((u8 *)unit + unit->vtable->delta90, 0, 0x11, 0xFE, -1);
                break;
            case 0x19:
                unit->vtable->fn90((u8 *)unit + unit->vtable->delta90, 1, 0x11, 0, 0);
                break;
            case 0x1A:
                unit->vtable->fn90((u8 *)unit + unit->vtable->delta90, 1, 0x11, 0, 0);
                unit->vtable->fn90((u8 *)unit + unit->vtable->delta90, 0, 0x12, 0xFE, 1);
                break;
            case 0x1B:
                unit->vtable->fn90((u8 *)unit + unit->vtable->delta90, 1, 0x11, 0, 0);
                unit->vtable->fn90((u8 *)unit + unit->vtable->delta90, 0, 0x12, 0xFE, 2);
                break;
            case 0x1C:
                if ((unit->flags1E >> 6) & 1) {
                    ((unit != 0) ? &unit->u78.c78 : 0)->x0 = 1;
                    vt = ((unit != 0) ? &unit->u78.c78 : 0)->vtable;
                    slot = &vt->slot08;
                    c = ((unit != 0) ? (u8 *)unit + 0x78 : 0);
                    slot->fn(c + slot->delta, 1);
                } else if ((unit->flags1E >> 3) & 1) {
                    u8 *comp;
                    ((unit != 0) ? &unit->u78.p.c84 : 0)->x0 = 1;
                    vt = unit->u78.p.c84.vtable;
                    comp = (u8 *)unit + 0x84;
                    vt->slot08.fn(comp + vt->slot08.delta, 1);
                }
                break;
            default:
                unit->vtable->fn90((u8 *)unit + unit->vtable->delta90, 0, kind, 0xFE, 0);
                break;
            }
        } else {
            switch (kind) {
            case 0x18:
            case 0x19:
            case 0x1A:
            case 0x1B:
                unit->vtable->fn90((u8 *)unit + unit->vtable->delta90, 1, 0x11, 0, 0);
                break;
            case 0x1C:
                if ((unit->flags1E >> 6) & 1) {
                    vt = ((unit != 0) ? &unit->u78.c78 : 0)->vtable;
                    slot = &vt->slot08;
                    c = ((unit != 0) ? (u8 *)unit + 0x78 : 0);
                    slot->fn(c + slot->delta, 0);
                } else if ((unit->flags1E >> 3) & 1) {
                    u8 *comp;
                    ((unit != 0) ? &unit->u78.p.c84 : 0)->x0 = 2;
                    vt = unit->u78.p.c84.vtable;
                    comp = (u8 *)unit + 0x84;
                    vt->slot08.fn(comp + vt->slot08.delta, 0);
                }
                break;
            default:
                unit->vtable->fn90((u8 *)unit + unit->vtable->delta90, 1, kind, 0, 0);
                break;
            }
        }
        func_80049CB4(0x1F, unit);
        func_80049CB4(2);
        break;
    }
    func_800485DC(D_801476B8);
}
