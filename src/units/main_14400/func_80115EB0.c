#include "common.h"
typedef struct { s32 field00; s32 field04; } Position;
typedef struct { unsigned char value; } Dir;
typedef struct Actor Actor;
typedef struct { unsigned char fields00[0x40]; short field40; s32 (*field44)(void *, Actor *, Position *, Position *, Dir *, Actor *, Actor *); } ActionMethods;
typedef struct { unsigned char field00; unsigned char field01; unsigned char fields02[6]; ActionMethods *field08; unsigned char field0C; unsigned char field0D; unsigned char field0E; } Action;
typedef struct { short adjustment; void (*method)(void *, s32); } ApplyMethod;
typedef struct { unsigned char fields00[0x80]; ApplyMethod field80; short field88; s32 (*field8C)(void *, Actor *); } ActorMethods;
struct Actor { unsigned char fields00[0x1E]; unsigned char field1E; unsigned char fields1F[5]; ActorMethods *field24; };
extern u32 D_8013960C;
extern unsigned short D_8015691A;
extern s32 func_80049CB4(s32 code, ...);
extern s32 func_800ADC90(Actor *source, Position *position, Position *destination);
extern Position *func_801168F8(Position *destination, Position *source);
extern Actor *func_80115E40(Action *action, Position *position);
extern s32 func_800A251C(Position *first, Position *second);
extern s32 func_800E1CD4(Actor *actor, s32 flags);
extern s32 func_80116230(Action *action, Position *position);
extern void *func_800C5F60(void);
extern void func_800C92F8(void *position);
static inline s32 positions_differ(Position *first, Position *second) {
    return func_800A251C(first, second) ^ 1;
}
/* unused_target is supplied by the original six-pointer call contract. */
s32 func_80115EB0(Action *action, Actor *actor, Position *position, Dir *direction, Actor *unused_target, Actor *source) {
    Position destination;
    Actor *selected;
    Actor *target;
    s32 event;
    s32 result;
    s32 moved;
    s32 check;
    s32 protected;
    s32 damage;
    ApplyMethod *apply;
    if (source != 0) {
        if (actor == 0) {
            func_80049CB4(0x1131);
            func_80049CB4(6);
            func_80049CB4(0xD7, position);
            func_80049CB4(7);
        }
        func_80049CB4(0xCB, source, position);
        func_80049CB4(0xE4, position);
        if ((((action->field0C >> 3) & 1) ^ 1) != 0) {
            func_80049CB4(0x129, 5);
            func_80049CB4(0xCD, source, position);
            func_80049CB4(2);
            func_800ADC90(source, position, position);
            source = 0;
        }
    } else {
        func_80049CB4(0xE4, position);
    }
    action->field0C = (action->field0C | 4) & 0xDF;
    if (action->field01 == 0xE8) {
        destination.field00 = position->field00;
        destination.field04 = position->field04;
    } else {
        func_801168F8(&destination, position);
    }
    target = func_80115E40(action, &destination);
    event = func_80049CB4(0xDA, &destination);
    if (event == -2) {
        D_8013960C = (D_8013960C * 2) & event;
    }
    selected = func_800A251C(&destination, position) ? source : 0;
    result = action->field08->field44((unsigned char *)action + action->field08->field40, actor, position, &destination, direction, target, selected);
    if (event == -2) {
        D_8013960C >>= 1;
    }
    if (positions_differ(&destination, position)) {
        result = 1;
    }
    check = 0;
    if (!((action->field0C >> 5) & 1)) {
        check = action->field01 != 0xE8;
    }
    if (check) {
        func_80049CB4(0xE3, position);
    }
    if (source != 0 && result != 0) {
        func_80049CB4(0xCD, source, position);
        func_80049CB4(2);
        func_800ADC90(source, position, position);
    }
    protected = 0;
    if (action->field01 == 0xDB && target != 0 && (target->field1E & 0x7C)) {
        protected = func_800E1CD4(target, 0x10) != 0;
    }
    moved = 0;
    if (protected == 0) {
        moved = func_80116230(action, position);
    }
    if (moved) {
        unsigned char flags;
        unsigned char column;
        action->field0E = position->field04;
        flags = action->field0C;
        column = position->field00;
        action->field0C = flags | 0x10;
        action->field0D = column;
    }
    if (actor != 0 && target != 0 && ((target->field1E >> 4) & 1)) {
        damage = target->field24->field8C((unsigned char *)target + target->field24->field88, actor) * D_8015691A / 100;
        apply = &actor->field24->field80;
        apply->method((unsigned char *)actor + apply->adjustment, damage ? damage : 1);
    }
    func_800C92F8(func_800C5F60());
    func_80049CB4(2);
    return moved;
}
