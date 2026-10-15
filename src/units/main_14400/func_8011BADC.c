#include "common.h"
#include "item_records.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef ItemPosition Pair;
typedef struct { s32 type; u8 pad[0xC]; Pair pos; } Msg;
extern u8 D_801476BC;
void *func_800B31E8(Pair *pos, s32 team);
s32 func_801131F8(Item *, Msg *);
s32 func_8011BADC(Item *s, Msg *msg) {
    s32 ok;
    switch (msg->type) {
    case 0x1F:
        ok = 0;
        if ((s->flags02 & 0x20) && !(s->payload.consumable.flags0C & 1)) {
            ok = func_800B31E8(&s->payload.consumable.position10, 10) == 0;
        }
        if (ok) {
            D_801476BC |= 2;
        }
        return 1;
    case 0x1A:
        s->payload.consumable.position10 = msg->pos;
        break;
    }
    return func_801131F8(s, msg);
}
