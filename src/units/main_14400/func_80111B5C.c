#include "common.h"
#include "item_records.h"
typedef unsigned char u8;
typedef short s16;
typedef struct { u8 pad00[0x1E]; u8 flags1E; } Unit;
typedef struct { s32 kind; void *actor; void *target; u8 detail0C[0x10]; Unit *unit1C; } Message;
extern s32 func_80049CB4(s32, ...);
extern void func_800498E4(s32, ...);
extern void *func_800B31E8(void *, s32);
extern s32 func_800E1CD4(Unit *, s32);
extern void func_80111DD8(Item *, Unit *);
extern void func_800A7B18(void *, void *, s32, s32);
extern void func_800D3650(void *);
extern s32 func_800AF28C(Item *, Message *);
static inline s32 message_kind(Message *message)
{
    return message->kind;
}
/* Kind-7 constructor func_80111530 installs D_8015D5E0, whose common prefix is ItemMethods. */
static inline ItemChargesMethods *methods(Item *item) { return (ItemChargesMethods *)item->field08; }
s32 func_80111B5C(Item *object, Message *message)
{
    Unit *target;
    char *name, *targetName;
    void *actor;
    s32 eligible, blocked;
    s32 kind = message_kind(message);
    switch (kind) {
    case 10: {
        Item *item = message->target;
        name = func_800AE674(object);
        targetName = func_800AE674(item);
        if (methods(object)->query5C((u8 *)object + methods(object)->delta58, message->actor, item)) {
            func_80049CB4(0x1134, 0);
            func_80049CB4(0x3F, message->actor);
            func_800498E4(0x93, name, targetName);
            func_80049CB4(0x136);
        }
        return 1;
    }
    case 5:
        methods(object)->use4C((u8 *)object + methods(object)->delta48, message->actor);
        return 1;
    case 18:
    case 19:
    case 20:
        eligible = 1;
        target = message->target;
        actor = message->actor;
        if (func_800B31E8(target, 10)) {
            eligible = 0;
        } else {
            blocked = !methods(object)->test44((u8 *)object + methods(object)->delta40) &&
                (target->flags1E & 0x7C) && func_800E1CD4(target, 15);
            if (blocked) eligible = 0;
        }
        if (eligible) {
            methods(object)->apply54((u8 *)object + methods(object)->delta50,
                actor, target, message->detail0C, message->unit1C);
            func_80111DD8(object, target);
        } else if (message->kind != 20) {
            func_800A7B18(target, actor, 1, 6);
        }
        if (message->kind == 18) {
            func_800D3650(object);
            if (object != 0) {
                object->field08->method0C((u8 *)object + object->field08->delta08, 3);
            }
        }
        return 1;
    default:
        return func_800AF28C(object, message);
    }
}
