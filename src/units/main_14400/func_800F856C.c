#include "common.h"

typedef struct {
    s32 type;
} Message;

typedef struct Object Object;

extern s32 func_800F85DC(Object *object);
extern s32 func_800F438C(void *actor, Message *msg);

/* Message handler (vtable D_80159C10 family): consume a few message types, defer the rest. */
s32 func_800F856C(void *self, Message *msg) {
    switch (msg->type) {
    case 0:
        return func_800F85DC(self);
    case 2:
        func_800F438C(self, msg);
        return 1;
    case 1:
    case 8:
    case 9:
    case 11:
    case 12:
    case 19:
    case 21:
        return 0;
    default:
        return func_800F438C(self, msg);
    }
}
