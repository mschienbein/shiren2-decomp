#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x, y; } Point;
typedef struct {
    u8 pad_00[8];
    signed short delta_08;
    u16 reserved_0A;
    void (*destroy_0C)(void *, s32);
} ItemVtable;
typedef struct { u8 pad_00[8]; ItemVtable *vtable_08; s32 mode_0C; } Item;
typedef struct {
    u8 pad_00[0x90];
    signed short delta_90;
    u16 reserved_92;
    s32 (*action_94)(void *, s32, s32, u8, s32);
} EntityVtable;
typedef struct {
    Point position;
    u8 pad_08[0x16];
    u8 flags_1E;
    u8 pad_1F[5];
    EntityVtable *vtable_24;
    u8 pad_28[0x54];
    u16 flags_7C;
} Entity;
typedef struct { s32 type; Entity *actor_04; Entity *target_08; } Event;
extern const u16 D_80156AB2, D_80156AB4;
extern u32 D_8013960C;
extern s32 func_800E9270(Entity *entity, Item *item, s32 code, s32 mode);
extern s32 func_800E0534(void *entity, s32 amount);
extern void func_800498E4(s32 id, ...);
extern void func_800E1ED0(Entity *entity);
extern void func_800E075C(Entity *entity);
extern s32 func_800E0C2C(Entity *entity);
extern s32 func_800EB350(Entity *entity);
extern s32 func_80049CB4(s32 id, ...);
extern void func_800D3650(void *item);
extern s32 func_80112B38(Item *item, Event *event);

static inline void destroy_item(Item *item)
{
    if (item != 0) {
        ItemVtable *vtable = item->vtable_08;
        vtable->destroy_0C((char *)item + vtable->delta_08, 3);
    }
}

static inline void point_copy(Point *out, Entity *entity)
{
    out->x = entity->position.x;
    out->y = entity->position.y;
}

static inline void entity_react(Entity *receiver)
{
    EntityVtable *vtable = receiver->vtable_24;
    vtable->action_94((char *)receiver + vtable->delta_90, 0, 0xC, 0xFE, 0);
}

s32 func_80126F74(Item *item, Event *event)
{
    Point position;
    Entity *entity;
    switch (event->type) {
        case 0: {
            s32 allowed = 0;
            u8 enabled;
            entity = event->actor_04;
            enabled = (entity->flags_1E >> 2) & 1;
            if (enabled) allowed = func_800E9270(entity, item, 0x6A, 0) != 0;
            if (allowed) {
                switch (item->mode_0C) {
                    case 1:
                        if ((signed short)func_800E0534(entity, D_80156AB2) == 0) {
                            func_800498E4(0x224);
                        } else {
                            func_80049CB4(0x128, 0x6C);
                        }
                        break;
                    case 2:
                        func_800E0534(entity, D_80156AB4);
                        func_800E1ED0(entity);
                        func_800498E4(0x11B);
                        func_80049CB4(0x128, 0x6D);
                        break;
                    default:
                        D_8013960C <<= 1;
                        func_800E075C(entity);
                        func_800E0C2C(entity);
                        func_800EB350(entity);
                        func_800E1ED0(entity);
                        D_8013960C >>= 1;
                        func_800498E4(0x11B);
                        func_80049CB4(0x128, 0x6D);
                        break;
                }
                func_80049CB4(0x1F, entity);
                destroy_item(item);
                return 1;
            }
            return 0;
        }
        case 0x12:
        case 0x13: {
            Entity *target = event->target_08;
            /* Kept as its own pointer, as in func_80124528's player copy. */
            Entity *receiver = target;
            s32 active;
            point_copy(&position, target);
            func_80049CB4(0x11B, &position);
            active = 0;
            if ((target->flags_1E >> 4) & 1) {
                active = (target->flags_7C >> 9) & 1;
            }
            if (active) entity_react(receiver);
            if (event->type == 0x12) {
                func_800D3650(item);
                destroy_item(item);
            }
            return 1;
        }
        default:
            return func_80112B38(item, event);
    }
}
