#include "common.h"
typedef struct { s32 x, y; } Point;
typedef struct { unsigned char pad0[2]; unsigned char field_2; } S;
typedef struct { s32 type; unsigned char pad4[12]; Point field_10; } Msg;
typedef struct { unsigned char pad0[0x1D]; unsigned char field_1D; } Actor;
void *func_800B4928(Point *pos);
s32 func_800A529C(void *self, s32 value);
s32 func_801131F8(S *self, Msg *message);
s32 func_8011BEBC(S *self, Msg *message) {
    if (message->type == 0x1A) {
        if (self->field_2 & 0x20) {
            Actor *actor = func_800B4928(&message->field_10);
            if (actor && (actor->field_1D >> 7)) func_800A529C(actor, 0);
        }
    }
    return func_801131F8(self, message);
}
