#include "common.h"

typedef struct { s32 kind; void *payload; } Event;

extern s32 func_800F438C(void *actor, Event *event);

static inline s32 event_kind(Event *event) { return event->kind; }

/* Message handler (entity table +0x5C): ignores events 0, 1, 21 and 25. */
s32 func_800F8A60(void *self, Event *msg) {
    switch (event_kind(msg)) {
    case 0:
    case 1:
    case 21:
    case 25:
        return 0;
    default:
        return func_800F438C(self, msg);
    }
}
