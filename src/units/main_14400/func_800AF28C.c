#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x, y; } Pos;
typedef struct { u8 value; } Dir;
typedef struct { u32 word; } Flags;
typedef struct { Pos position; Dir direction; u8 pad_09[0x17]; Flags flags_20; } Actor;
typedef struct { u8 pad_00[8]; short delta_08, slot_0A; void (*destroy_0C)(void *, s32); } ItemTable;
typedef struct { u8 pad_00[8]; ItemTable *vtable_08; } Item;
typedef struct { s32 kind; Actor *owner; Actor *target; Dir direction; u8 pad_0D[3]; Pos position; s32 limit_or_flags; s32 flags; } Event;
/* The live operation occupies original sp+0x28..sp+0x148, including the
 * storage consumed by the constructor and iterative executor. */
typedef struct { u8 storage[0x120]; } Operation;
extern u16 func_800AF1DC(Item *, void *, void *, s32 *);
extern Operation *func_800C32C0(Operation *, void *, Item *, Pos *, Dir *, s32, s32, u16);
extern void func_800C2D0C(void *);
extern void func_800D3650(void *);
extern void func_800A7B18(void *, void *, s32, s32);
extern char *func_800AE674(void *);
extern s32 func_800ADC90(void *, Pos *, void *);
extern void func_800498E4(s32, ...);
/* ODD_C: Read the copied flag object through its original addressable view. */
static inline s32 flag_test(const Flags *flags, s32 bit) { return (flags->word >> bit) & 1; }
/* ODD_C: Combine full event words before narrowing the operation flags. */
static inline u16 event_bits(s32 flags, u16 result) { return result | flags; }
s32 func_800AF28C(Item *item, Event *event) {
    Pos position;
    Operation operation;
    Flags flags;
    s32 actor_limit, point_limit;
    switch (event->kind) {
    case 9: {
        Actor *actor = event->owner;
        Pos *point = &position;
        u16 enabled;
        u16 result;
        position.x = actor->position.x;
        point->y = actor->position.y;
        flags.word = actor->flags_20.word;
        enabled = flag_test(&flags, 1);
        result = func_800AF1DC(item, actor, point, &actor_limit);
        func_800C32C0(&operation, actor, item, point, &actor->direction, actor_limit, enabled,
                       event_bits(event->limit_or_flags, result));
        func_800C2D0C(&operation);
        return 1;
    }
    case 17: {
        Actor *actor = event->owner;
        Pos *point = &position;
        u16 result;
        position.x = event->position.x;
        point->y = event->position.y;
        result = event_bits(event->flags, func_800AF1DC(item, 0, point, &point_limit));
        if (event->limit_or_flags != -1) point_limit = event->limit_or_flags;
        func_800C32C0(&operation, actor, item, point, &event->direction, point_limit, 0, result);
        func_800C2D0C(&operation);
        return 1;
    }
    case 18:
        func_800D3650(item);
        if (item) item->vtable_08->destroy_0C((u8 *)item + item->vtable_08->delta_08, 3);
        /* Destruction is followed by the same event-target damage as case 19. */
    case 19:
        func_800A7B18(event->target, event->owner, 1, 6);
        return 1;
    case 26:
        return 1;
    case 27: {
        char *name = func_800AE674(item);
        s32 failed = func_800ADC90(item, &event->position, &event->position) != 1;
        if (failed) func_800498E4(0x87, name);
        return 1;
    }
    }
    return 0;
}
