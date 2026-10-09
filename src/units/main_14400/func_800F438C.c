#include "common.h"
typedef struct { s32 kind; void *payload; } Event;
extern s32 func_800F442C(void *actor);
extern s32 func_800F4498(void *actor);
extern s32 func_800F45D4(void *actor, void *payload);
extern void func_800F4608(void *actor, Event *event);
extern s32 func_800E4B60(void *actor, void *event);
static inline s32 event_kind(Event *event) { return event->kind; }
s32 func_800F438C(void *actor, Event *event) {
    switch (event_kind(event)) {
    case 0: return func_800F442C(actor);
    case 1: return func_800F4498(actor);
    case 25: return func_800F45D4(actor, event->payload);
    case 9: func_800F4608(actor, event); return 1;
    default: return func_800E4B60(actor, event);
    }
}
