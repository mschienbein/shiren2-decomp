#include "common.h"
typedef struct { s32 x, y; } Pos;
typedef struct { s32 type; void *arg; char pad_08[0x10]; Pos pos; } Event;
typedef struct { short delta; short index; s32 (*fn)(void *, void *); } Entry;
typedef struct { char pad_00[0x38]; Entry message; } Table;
typedef struct { char pad_00[8]; Table *table; } Object;
void func_800AE518(Object *self, void *arg, s32 x, s32 y) {
    Event event;
    Event *message = &event;
    Entry *entry;
    event.type = 15;
    event.arg = arg;
    message->pos.x = x;
    message->pos.y = y;
    entry = &self->table->message;
    entry->fn((char *)self + entry->delta, message);
}
