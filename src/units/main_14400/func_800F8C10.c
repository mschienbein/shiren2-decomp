#include "common.h"

typedef struct Msg800F8C10 {
    s32 type_00;
    void *target_04;
} Msg800F8C10;

/* Reads only the receiver. */
s32 func_800F8C68(void *self);
s32 func_800F8CAC(void *self, void *target);
s32 func_800F438C(void *self, void *msg);

/* Virtual message handler (vtable slot at D_80159E4C). */
s32 func_800F8C10(void *self, Msg800F8C10 *msg)
{
    switch (msg->type_00) {
    case 0:
        return func_800F8C68(self);
    case 0xD:
        return func_800F8CAC(self, msg->target_04);
    default:
        return func_800F438C(self, msg);
    }
}
