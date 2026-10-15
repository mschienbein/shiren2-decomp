#include "common.h"
#include "item_records.h"

typedef unsigned char u8;
typedef struct { s32 x, y; } Pos;
typedef struct { u8 pad00[0x1E]; u8 flags1E; } Actor;
/* Kind 2 constructor 80112D20 installs D_8015D730. */
typedef struct {
    ItemMethods base;
    short delta40, index42;
    void (*apply44)(void *, Actor *, void *);
} ItemConsumableMethods;
typedef struct { s32 kind; Actor *actor; void *item; s32 field0C; Pos position10; } Msg;
extern s32 func_80112F24(Item *self, Actor *actor);
extern s32 func_800A692C(Actor *actor, s32 kind);
extern s32 func_80049CB4(s32 id, ...);
extern void func_800498E4(s32 id, ...);
extern void func_800D3650(void *arg);
extern u32 func_800B1C6C(Pos *pos);
extern void func_80112E7C(Item *self);
extern void func_80112EAC(Item *self);
extern s32 func_800AD468(u32 bit);
extern s32 func_800AF28C(Item *self, Msg *msg);

static inline s32 event_kind(Msg *msg) { return msg->kind; }

s32 func_801131F8(Item *self, Msg *msg)
{
    Actor *actor;
    void *item;
    switch (event_kind(msg)) {
    case 1:
    case 2:
        item = 0;
        actor = msg->actor;
        if (func_80112F24(self, actor)) {
            if (func_800A692C(actor, 10)) {
                func_80049CB4(0x132);
                func_800498E4(0x225);
            } else {
                if (msg->kind == 2) item = msg->item;
                ((ItemConsumableMethods *)self->field08)->apply44((char *)self + ((ItemConsumableMethods *)self->field08)->delta40, actor, item);
            }
            if ((actor->flags1E >> 2) & 1) func_80049CB4(0x136);
            func_800D3650(self);
            if (self != 0) self->field08->method0C((char *)self + self->field08->delta08, 3);
            return 1;
        } else return 0;
    case 30:
        self->payload.consumable.flags0C &= 0xFE;
        return 1;
    case 26:
        if (func_800B1C6C(&msg->position10) & 0x2000) {
            func_80112E7C(self);
        } else if (self->field08->test1C((char *)self + self->field08->delta18, 0x22)) {
            if (func_800AD468(self->field01)) func_80112EAC(self);
        }
        return 1;
    default:
        break;
    }
    return func_800AF28C(self, msg);
}
