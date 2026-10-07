#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef struct { char pad[0x38]; s16 offset38; char pad3A[2]; void *(*func3C)(void *, u32); } VTable;
typedef struct { char pad[0x38]; s16 offset38; char pad3A[2]; s32 (*func3C)(void *, void *); } EventVTable;
typedef struct { s32 unk0; VTable *vtbl4; EventVTable *vtbl8; } Obj;
typedef struct { s32 x; s32 y; } Vec2;
typedef struct { Vec2 pos; u8 unk8; } Target;
typedef struct { s32 id; void *arg; } Msg;
typedef struct {
    s32 type;
    Target *target;
    void *sender;
    s32 padC;
    Vec2 pos;
    s32 unk18;
    s32 pad1C;
} Event;
s32 func_80049CB4(s32, ...);
char *func_800A3B20(void *obj);
void func_800497F0(s32 message_id, ...);
Obj *func_8011422C(void *);
void func_80122A80(void *, void *);
s32 func_80114E28(void *, Msg *);
static inline void Event_init(Event *event, Target *target, void *sender, s32 kind) {
    event->type = 0x1B;
    event->target = target;
    event->sender = sender;
    event->pos = target->pos;
    event->unk18 = kind;
}
s32 func_80122C2C(void *self, Msg *msg) {
    switch (msg->id) {
    case 10:
        func_80122A80(self, msg->arg);
        return 1;
    case 13: {
        Target *target = msg->arg;
        Obj *obj;
        Obj *receiver;
        Event event;
        s32 value = func_80049CB4(0x109A, target);
        func_800497F0(0xBD, value, func_800A3B20(target));
        obj = func_8011422C(self);
        receiver = obj->vtbl4->func3C((char *)obj + obj->vtbl4->offset38, 0);
        Event_init(&event, target, self, target->unk8);
        return receiver->vtbl8->func3C((char *)receiver + receiver->vtbl8->offset38, &event);
    }
    default:
        return func_80114E28(self, msg);
    }
}
