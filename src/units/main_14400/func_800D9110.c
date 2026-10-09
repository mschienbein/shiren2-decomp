#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef struct { u8 value; } Byte;
/* Actor messages have a 24-byte variant payload; type 1 needs only the tag. */
typedef struct { s32 kind; u8 payload[20]; } Message;
typedef struct { s16 delta, index; s32 (*call)(void *, Message *); } MessageSlot;
typedef struct { u8 pad_00[0x58]; MessageSlot message; } VTable;
typedef struct {
    u8 pad_00[8]; Byte direction; u8 pad_09[0x15]; u8 flags_1E;
    u8 pad_1F[5]; VTable *vtable_24; u8 pad_28[0xDC]; void *link_104;
} Actor;
typedef struct { u8 pad_00; u8 kind; u8 pad_02[10]; u16 id; } Item;
extern Actor *func_800C5F60(void);
extern Item *func_800A6D18(Actor *self);
extern s32 func_800E2044(Actor *self);
extern s32 func_801290F4(Item *item, Byte direction, s32 flag);
extern void func_801E8A84(s32 id);
extern void func_801F212C(u16 id, u8 flag);
extern s32 func_800E1D14(Actor *self, s32 kind);
extern u8 func_800C57CC(void *rng, s32 limit);
extern void func_800A665C(Actor *obj, u8 *value);
extern void func_8011B998(void);
extern void func_8006D418(void);

extern u8 D_801476BC;
extern u8 D_80147620[];

static inline s32 controls_disabled(SelectionRecord *record) {
    return (record->flags >> 2) & 1;
}

static inline s32 controls_enabled(SelectionRecord *record) {
    return controls_disabled(record) ^ 1;
}

/* The command +0x14 slot supplies command; this override uses the player instead. */
s32 func_800D9110(void *command) {
    Message message;
    Byte direction;
    Actor *self = func_800C5F60();
    Item *item = func_800A6D18(self);
    s32 special = 0;
    if (item && item->kind == 0xF4) special = func_800E2044(self) != 0;
    if (special) {
        s32 ready = 0;
        if ((self->flags_1E >> 2) & 1) ready = self->link_104 == 0;
        if (ready && func_801290F4(item, self->direction, 1)) {
            func_801E8A84(0x1B);
            func_801E8A84(0x17);
            func_801F212C(item->id, 0);
            func_8006D418();
            return 1;
        }
    }
    if (controls_enabled(&D_80142F18)) {
        if (func_800E1D14(self, 0x13)) {
            direction.value = func_800C57CC(D_80147620, 7) & 7;
            func_800A665C(self, &direction.value);
        }
        message.kind = 1;
        if (self->vtable_24->message.call((u8 *)self + self->vtable_24->message.delta, &message)) {
            if (((self->flags_1E >> 2) & 1) && (D_801476BC & 2)) func_8011B998();
            return 3;
        }
    }
    func_8006D418();
    return 1;
}
