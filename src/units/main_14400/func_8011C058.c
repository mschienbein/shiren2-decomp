#include "common.h"
#include "item_records.h"
typedef unsigned char u8;
typedef ItemPosition Pair;
typedef struct { s32 type; char pad4[0xC]; Pair pos; } Msg;
extern u8 D_801476BC;
extern void *func_800B31E8(Pair *pos, s32 team);
extern s32 func_801131F8(Item *, Msg *);
s32 func_8011C058(Item *self, Msg *m) {
    switch (m->type) {
    case 0x1F: {
        s32 hit = 0;
        if (!(self->payload.consumable.flags0C & 1) && (self->flags02 & 0x20)) hit = !func_800B31E8(&self->payload.consumable.position10, 10);
        if (hit) D_801476BC |= 1;
        return 1;
    }
    case 0x1A:
        self->payload.consumable.position10 = m->pos;
        break;
    }
    return func_801131F8(self, m);
}
