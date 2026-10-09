#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 kind; void *owner; u8 rest[0x18]; } Event;
typedef struct { u8 pad_00[0x38]; short delta_38, index_3A; s32 (*event_3C)(void *, Event *); } Methods;
typedef struct { u8 field_00, kind_01; u8 pad_02[6]; Methods *table_08; } Item;
typedef struct { u8 bytes[0x14]; } Action;
extern s32 func_800EE504(void *);
extern s32 func_800E1CC4(void *, s32);
extern s32 func_800A692C(void *, s32);
extern void *func_800A4B24(void *, s32, s32, s32);
extern u8 func_800A6420(void *, void *);
extern void *func_800A65E4(u8 *, void *, void *);
extern void func_800A665C(void *, u8 *);
extern void *func_8010AB7C(void *, void *);
extern s32 func_800A6EE0(void *);
extern void *func_800C4BC0(void *, void *, void *, u16);
extern void func_800C4864(void *, s32, void *);
extern s32 func_800ADC90(void *, void *, void *);
static inline Event *event_init(Event *event, s32 kind, void *owner) {
    event->kind = kind;
    event->owner = owner;
    return event;
}

s32 func_8010AEB8(void *actor, void *selected, Item *item) {
    union { Event event; Action action; } message;
    u8 direction;
    s32 blocked = 0;
    if (!func_800EE504(actor) || func_800E1CC4(actor, 0) || func_800A692C(actor, 0x12)) blocked = 1;
    if (!blocked && selected) {
        void *target = 0; /* facing target, reused for the destination from func_8010AB7C */
        s32 immediate = 0;
        u8 kind = item->kind_01;
        switch (kind) {
        case 5: case 21: {
            u8 relation;
            target = func_800A4B24(actor, 2, 1, 1);
            relation = func_800A6420(actor, target);
            immediate = kind == 5 ? relation == 0 : relation != 3;
            break;
        }
        case 1: case 2: case 3: case 6: case 11: case 14: case 18: case 19:
            immediate = 1;
            break;
        }
        if (immediate) {
            s32 refused;
            func_800A65E4(&direction, actor, target);
            func_800A665C(actor, &direction);
            refused = item->table_08->event_3C((u8 *)item + item->table_08->delta_38, event_init(&message.event, 0, actor)) != 1;
            if (refused)
                func_800ADC90(item, actor, actor);
        } else {
            target = func_8010AB7C(actor, selected);
            if (target) {
                func_800C4BC0(&message.action, actor, item, (u16)func_800A6EE0(actor));
                func_800C4864(&message.action, 0x29, target);
            } else {
                func_800ADC90(item, actor, actor);
            }
        }
        return 1;
    }
    return 0;
}
