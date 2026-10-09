#include "common.h"

typedef unsigned char u8;

typedef struct {
    s32 type;
} Message;

s32 func_800F8834(u8 *self);
s32 func_800F438C(u8 *self, Message *msg);

/* Slot-11 message handler of vtable D_80159CB0. */
s32 func_800F87EC(u8 *self, Message *msg) {
    switch (msg->type) {
    case 0:
        return func_800F8834(self);
    case 13:
        return 0;
    default:
        return func_800F438C(self, msg);
    }
}
