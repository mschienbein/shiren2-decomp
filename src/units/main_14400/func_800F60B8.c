#include "common.h"
typedef struct Object Object;
typedef struct { Object *source; s32 kind, field_08; short amount; unsigned short flags; } Damage;
typedef struct { u32 kind; Object *target; s32 reserved_08[2]; Damage *damage; } Event;
extern s32 func_800F438C(Object *, Event *);
extern void func_800F6038(Object *, s32);
extern s32 func_800F63F0(Object *), func_800F6644(Object *);
extern void func_800F6694(Object *, Event *);
extern s32 func_800F66C4(Object *, void *);
extern void func_800F68A4(Object *, Object *);
s32 func_800F60B8(Object *self, Event *event) {
    switch (event->kind) {
    case 0: return func_800F63F0(self);
    case 1: return func_800F6644(self);
    case 9: {
        Damage *damage = event->damage;
        if (damage->amount > 0 && (u32)(damage->kind - 7) >= 13) {
            func_800F68A4(self, damage->source);
            func_800F6038(self, 1);
        }
        break;
    }
    case 10: func_800F6694(self, event); return 1;
    case 13: return func_800F66C4(self, event->target);
    case 17: func_800F68A4(self, event->target); return 1;
    }
    return func_800F438C(self, event);
}
