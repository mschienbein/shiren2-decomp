#include "common.h"
typedef unsigned char u8;
typedef struct { s32 field_00, active_04; u8 fields_08[0x28]; void *table_30; } Component;
typedef struct { u8 fields_00[0x78]; Component component_78; } Actor;
typedef struct { u32 kind; void *target; } Event;
extern s32 func_801F2B48(Component *, Actor *);
extern void func_800EFAB4(Actor *, void *);
extern s32 func_800E4B60(Actor *, Event *);
s32 func_800EFA38(Actor *actor, Event *event) {
    switch (event->kind) {
    case 0: return func_801F2B48(actor ? &actor->component_78 : 0, actor);
    case 8: case 9: case 10: case 11: case 12: case 19: case 21: return 0;
    case 13: func_800EFAB4(actor, event->target);
    case 1: return 1;
    default: return func_800E4B60(actor, event);
    }
}
