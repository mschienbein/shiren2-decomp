#include "common.h"
typedef unsigned char u8;
typedef struct Object Object;
typedef struct { Object *source; s32 kind, field_08; short amount; unsigned short flags; } Damage;
typedef struct { u32 kind; Object *target; s32 reserved_08[2]; Damage *damage; } Event;
typedef struct { s32 count; void *table; u8 reserved_08[8]; Object *owner; u8 reserved_14[0xC]; } Inventory;
struct Object { u8 reserved_00[0x72]; u8 flags_72; u8 reserved_73[0x19]; Inventory inventory_8C; };
extern s32 func_800F438C(Object *, Event *);
extern s32 func_800F7474(Object *), func_800F7664(Object *);
extern void func_800F7798(Object *, Event *), func_800E20F0(Object *), func_800CF550(Inventory *);
extern s32 func_800F7884(Object *, void *);
extern void func_800F7948(Object *, Object *);
s32 func_800F7370(Object *self, Event *event) {
    switch (event->kind) {
    case 0: return func_800F7474(self);
    case 1: return func_800F7664(self);
    case 9:
        if (event->damage->amount > 0) func_800F7948(self, event->damage->source);
        if (self->flags_72 & 1) func_800E20F0(self);
        break;
    case 10: func_800F7798(self, event); return 1;
    case 12: func_800CF550(&self->inventory_8C); return 1;
    case 13: return func_800F7884(self, event->target);
    case 17: func_800F7948(self, event->target); return 1;
    }
    return func_800F438C(self, event);
}
