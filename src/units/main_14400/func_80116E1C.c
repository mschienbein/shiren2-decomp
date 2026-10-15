#include "common.h"
#include "item_records.h"
typedef unsigned char u8;
typedef struct { s32 x; s32 y; } Point;
typedef struct { Point position; u8 pad_8[0x16]; u8 field_1E; } Entity;
/* Kind 1 constructor 80116D50 installs D_8015DA98. */
typedef struct { ItemMethods base; short action_delta; short index42; void (*action)(void *, Entity *); short pair_delta; short index4A; void (*pair_action)(void *, Entity *, Entity *); } ItemFoodMethods;
typedef struct { s32 kind; Entity *first; Entity *second; } Message;
extern s32 func_800E9270(Entity *, Item *, s32, s32);
extern void func_800ACDD8(Item *);
extern void func_800EB3CC(Entity *, short);
extern s32 func_800E1CD4(Entity *, s32);
extern s32 func_80049CB4(s32, ...);
extern void func_800D3650(void *);
extern s32 func_800AF28C(Item *, Message *);
extern u32 D_8013960C;
extern short D_801569F4;
static inline void destroy(Item *actor) { if (actor) actor->field08->method0C((u8 *)actor + actor->field08->delta08, 3); }
static inline s32 message_kind(Message *message) { return message->kind; }
static inline s32 blocked(Entity *target) {
    return (target->field_1E & 0x7C) && func_800E1CD4(target, 15);
}
static inline Point *copy_point(Point *out, Point *in) {
    out->x = in->x;
    out->y = in->y;
    return out;
}
s32 func_80116E1C(Item *actor, Message *message) {
    Entity *target; Point point;
    switch (message_kind(message)) {
    case 0: {
        Entity *first = message->first;
        if (!func_800E9270(first, actor, 0x6A, 0)) return 0;
        target = first;
        if (target->field_1E & 0xC) {
            func_800ACDD8(actor);
            if ((target->field_1E >> 2) & 1) {
                D_8013960C <<= 1;
                func_800EB3CC(target, D_801569F4);
                D_8013960C >>= 1;
            }
        }
        func_80049CB4(0x129, 10);
        func_80049CB4(0xC5, actor);
        ((ItemFoodMethods *)actor->field08)->action((u8 *)actor + ((ItemFoodMethods *)actor->field08)->action_delta, target);
        destroy(actor);
        return 1;
    }
    case 18:
        if (blocked(message->second)) break;
        /* fall through */
    case 19:
        target = message->second;
        if (target->field_1E & 0x7C) {
            func_80049CB4(0x10F, copy_point(&point, &target->position));
            ((ItemFoodMethods *)actor->field08)->pair_action((u8 *)actor + ((ItemFoodMethods *)actor->field08)->pair_delta, message->first, target);
            if (message->kind == 18) {
                func_800D3650(actor);
                destroy(actor);
            }
            return 1;
        }
        break;
    }
    return func_800AF28C(actor, message);
}
