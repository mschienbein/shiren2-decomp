#include "common.h"
typedef unsigned char u8;
typedef short s16;
typedef struct { u8 pad00[0x1E]; u8 flags1E; } Unit;
typedef struct { s16 delta, index; void (*call)(void *, s32); } DestroyEntry;
typedef struct { s16 delta, index; s32 (*call)(void *); } TestEntry;
typedef struct { s16 delta, index; void (*call)(void *, void *); } UseEntry;
typedef struct { s16 delta, index; void (*call)(void *, void *, Unit *, void *, Unit *); } ApplyEntry;
typedef struct { s16 delta, index; s32 (*call)(void *, void *, Unit *); } QueryEntry;
typedef struct { u8 pad00[8]; DestroyEntry destroy08; u8 pad10[0x30]; TestEntry test40; UseEntry use48; ApplyEntry apply50; QueryEntry query58; } Vtable;
typedef struct { u8 pad00[8]; Vtable *vtable; } Object;
typedef struct { s32 kind; void *actor; Unit *target; u8 detail0C[0x10]; Unit *unit1C; } Message;
extern char *func_800AE674(void *);
extern s32 func_80049CB4(s32, ...);
extern void func_800498E4(s32, ...);
extern void *func_800B31E8(void *, s32);
extern s32 func_800E1CD4(Unit *, s32);
extern void func_80111DD8(Object *, Unit *);
extern void func_800A7B18(void *, void *, s32, s32);
extern void func_800D3650(void *);
extern s32 func_800AF28C(Object *, Message *);
static inline s32 message_kind(Message *message)
{
    return message->kind;
}
s32 func_80111B5C(Object *object, Message *message)
{
    Unit *target;
    char *name, *targetName;
    void *actor;
    s32 eligible, blocked;
    s32 kind = message_kind(message);
    switch (kind) {
    case 10:
        target = message->target;
        name = func_800AE674(object);
        targetName = func_800AE674(target);
        if (object->vtable->query58.call((u8 *)object + object->vtable->query58.delta, message->actor, target)) {
            func_80049CB4(0x1134, 0);
            func_80049CB4(0x3F, message->actor);
            func_800498E4(0x93, name, targetName);
            func_80049CB4(0x136);
        }
        return 1;
    case 5:
        object->vtable->use48.call((u8 *)object + object->vtable->use48.delta, message->actor);
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
            blocked = !object->vtable->test40.call((u8 *)object + object->vtable->test40.delta) &&
                (target->flags1E & 0x7C) && func_800E1CD4(target, 15);
            if (blocked) eligible = 0;
        }
        if (eligible) {
            object->vtable->apply50.call((u8 *)object + object->vtable->apply50.delta,
                actor, target, message->detail0C, message->unit1C);
            func_80111DD8(object, target);
        } else if (message->kind != 20) {
            func_800A7B18(target, actor, 1, 6);
        }
        if (message->kind == 18) {
            func_800D3650(object);
            if (object != 0) {
                object->vtable->destroy08.call((u8 *)object + object->vtable->destroy08.delta, 3);
            }
        }
        return 1;
    default:
        return func_800AF28C(object, message);
    }
}
