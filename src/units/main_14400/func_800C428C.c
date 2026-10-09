#include "common.h"

typedef unsigned char u8;
typedef signed short s16;

typedef struct {
    s32 x;
    s32 y;
} Position800C428C;

/* 0x20-byte event: kind +0x00, code byte +0x0C, map position +0x10. */
typedef struct {
    s32 kind;
    s32 field_04;
    s32 field_08;
    u8 code;
    u8 pad_0D[3];
    Position800C428C pos;
    s32 field_18;
    s32 field_1C;
} Event800C428C;

/* Decided message-handler slot (+0x3C of the table at +8): s32 (void *receiver, void *event). */
typedef struct {
    s16 delta;
    s16 index;
    s32 (*handle)(void *receiver, void *event);
} VtblEntry800C428C;
typedef struct {
    u8 pad_00[0x38];
    VtblEntry800C428C message;
} Vtbl800C428C;
typedef struct {
    s32 field_00;
    s32 field_04;
    Vtbl800C428C *vtbl;
} Entity800C428C;

/* Partial view: enable word +0xB8, count +0xBC, positions +0xC0, codes +0x110. */
typedef struct {
    u8 pad_00[0xB8];
    s32 enabled;
    u8 count;
    u8 pad_BD[3];
    Position800C428C positions[10];
    u8 codes[10];
} Owner800C428C;

/* Entity standing at a map position, or null. */
void *func_800B4D80(Position800C428C *pos);

/* ODD_C: fills the 0x15 event from pointers to the stored position and code (the original
 * computes both element addresses before copying); this also shapes the address registers. */
static inline void set_event(Event800C428C *event, Position800C428C *pos, u8 *code) {
    event->kind = 0x15;
    event->pos = *pos;
    event->code = *code;
}

/* Send event 0x15 to the entity at each stored position. */
void func_800C428C(Owner800C428C *owner) {
    s32 i;
    Event800C428C event;
    Entity800C428C *target;

    if (owner->enabled == 0) return;
    i = 0;
    while (1) {
        if (i >= owner->count) break;
        target = func_800B4D80(&owner->positions[i]);
        if (target != 0) {
            set_event(&event, &owner->positions[i], &owner->codes[i]);
            target->vtbl->message.handle((u8 *)target + target->vtbl->message.delta, &event);
        }
        i++;
    }
}
