#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct Damage Damage;

/* Entity message: type word, payload; type 9 carries a Damage record at 0x10. */
typedef struct {
    s32 type;
    u8 fields_4[0xC];
    Damage *damage10;
} Message;

/* g++ 2.x vtable slot 11 (message handler, e.g. func_800F27A4/func_800F87EC). */
typedef struct {
    s16 delta;
    s16 index;
    s32 (*fn)(void *self, Message *msg);
} VtblEntry;

typedef struct {
    u8 pad0[0x24];
    VtblEntry *vtbl24;
} Entity;

void func_800A7ADC(Entity *entity, Damage *damage) {
    Message message;
    Message *msg = &message;
    VtblEntry *entry;

    msg->type = 9;
    msg->damage10 = damage;
    entry = &entity->vtbl24[11];
    entry->fn((u8 *)entity + entry->delta, msg);
}
