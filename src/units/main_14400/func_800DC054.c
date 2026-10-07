#include "common.h"

typedef unsigned char u8;
typedef struct { short offset; short pad; void *fn; } VEntry;
typedef struct { s32 f0; VEntry *vtbl; } ObjA;
typedef struct { u8 kind; } Target;
typedef struct { s32 f0; s32 f4; VEntry *vtbl; } ObjB;
typedef struct { ObjA *obj; Target *target; } Slot;
typedef struct { char pad[8]; ObjA *obj1; ObjB *actor; Slot slot; } Self;
typedef struct { s32 id; void *player; Target *target; s32 pad[5]; } Msg;
extern void *D_801476B8;
void func_800498E4(s32, ...);
void func_800DAD20(Self*, void*);
void func_800D02AC(Slot*);
void func_800DAD80(Self*);
s32 func_800DC054(Self *self){
    s32 failed = 0;
    Slot *slot;
    Target *target;
    ObjB *actor;
    Msg msg;
    Msg *pmsg;
    if (!((s32 (*)(void*, void*, s32))self->obj1->vtbl[12].fn)((char*)self->obj1 + self->obj1->vtbl[12].offset, self->actor, 1)
        || !((s32 (*)(void*, void*, s32))self->slot.obj->vtbl[12].fn)((char*)self->slot.obj + self->slot.obj->vtbl[12].offset, self->slot.target, 1)) {
        failed = 1;
    }
    if (failed) return 0;
    slot = &self->slot;
    target = slot->target;
    if (target->kind != 1) {
        func_800498E4(0xE5);
        return 0;
    }
    actor = self->actor;
    func_800DAD20(self, target);
    func_800DAD20(self, actor);
    msg.id = 10;
    pmsg = &msg;
    pmsg->target = target;
    pmsg->player = D_801476B8;
    if (((s32 (*)(void*, Msg*))actor->vtbl[7].fn)((char*)actor + actor->vtbl[7].offset, pmsg)) {
        func_800D02AC(slot);
    }
    func_800DAD80(self);
    return 0;
}
