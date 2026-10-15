#include "common.h"
#include "item_records.h"
typedef unsigned char u8;
typedef struct { u8 pad_00[0x1E]; u8 flags_1E; } Actor;
typedef struct { s32 kind; Actor *source; Actor *target; u8 pad_0C[0x10]; void *origin; } Event;
extern short D_80156992, D_80156994;
extern void func_800498E4(s32, ...);
extern s32 func_80049CB4(s32, ...);
extern char *func_800A3B20(void *);
extern void func_800D3650(void *);
extern s32 func_800E9270(void *, Item *, s32, s32);
extern void func_800EB3CC(void *, short);
extern s32 func_8010BAE8(void *, s32);
extern s32 func_8010BEC4(void *, u8);
extern s32 func_8010C47C(Item *, void *);
extern void func_801104D0(Item *, void *);
extern void func_801106BC(void *, void *);
extern void func_80111128(void *, void *, void *, void *);
static inline void destroy(Item *item) {
    if (item) item->field08->method0C((u8 *)item + item->field08->delta08, 3);
}
s32 func_80110258(Item *item, Event *event) {
    switch (event->kind) {
    case 16:
        func_801106BC(item, event->source);
        return 1;
    case 15:
        func_801104D0(item, event);
        return 1;
    case 3: {
        Actor *actor = event->source;
        /* ODD_C: a second name for the actor, taken before the first branch, keeps the
           consumption target in its own saved register as in the original. */
        Actor *target = actor;
        if ((actor->flags_1E >> 2) & 1) {
            s32 allowed;
            if (item->flags02 & 4) func_80049CB4(0x1085, actor, item);
            allowed = 0;
            if ((u8)func_8010BEC4(item, 0x3F)) allowed = func_800E9270(actor, item, 0x18, 0) != 0;
            if (allowed) {
                func_800EB3CC(target, D_80156994);
                destroy(item);
                return 1;
            }
        }
        return 0;
    }
    case 4: {
        Actor *source = event->source;
        /* ODD_C: the flag test reads the loaded source; the rest of the case uses a copy
           taken before the branch, keeping the two in separate registers. */
        Actor *actor = source;
        s32 allowed = 0;
        if ((source->flags_1E >> 2) & 1) allowed = (u8)func_8010BEC4(item, 0x42) != 0;
        if (allowed) {
            if (item->payload.seals.field0C + item->payload.seals.field0D > 0) {
                char *name;
                func_80049CB4(0x1134, 0);
                func_80049CB4(0x2F, actor, item);
                func_80049CB4(0x129, 3);
                name = func_800A3B20(actor);
                func_800498E4(0x19, name, func_800AE674(item));
                func_8010BAE8(item, -1);
                func_800EB3CC(actor, D_80156992);
                func_80049CB4(0x136);
            } else func_800498E4(0x1A, func_800AE674(item));
            return 1;
        }
        return 0;
    }
    case 18:
        func_80111128(item, event->source, event->origin, event->target);
        func_800D3650(item);
        destroy(item);
        return 1;
    case 19:
        func_80111128(item, event->source, event->origin, event->target);
        return 1;
    default:
        return func_8010C47C(item, event);
    }
}
