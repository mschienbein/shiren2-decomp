#include "common.h"
#include "item_records.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x, y; } Point;
typedef struct { s32 type; unsigned char pad4[12]; Point field_10; } Msg;
typedef struct { unsigned char pad0[0x1C]; u16 flags_1C; } Actor;
void *func_800B4928(Point *pos);
s32 func_800A529C(void *self, s32 value);
s32 func_801131F8(Item *self, Msg *message);
s32 func_8011BEBC(Item *self, Msg *message) {
    if (message->type == 0x1A) {
        if (self->flags02 & 0x20) {
            Actor *actor = func_800B4928(&message->field_10);
            if (actor && ((u8)actor->flags_1C >> 7)) func_800A529C(actor, 0);
        }
    }
    return func_801131F8(self, message);
}
