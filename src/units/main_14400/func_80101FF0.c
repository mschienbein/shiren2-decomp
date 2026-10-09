#include "common.h"
typedef unsigned char u8;
typedef struct Item Item;
typedef struct Unit Unit;
typedef struct { s32 kind; Unit *source; void *target; u8 direction; u8 padD[0xF]; Unit *repeat; } Message;
typedef struct { u8 pad0[0x10]; short delta10, pad12; s32 (*locked14)(void *); short delta18, pad1A; s32 (*query1C)(void *, s32); u8 pad20[0x18]; short delta38, pad3A; s32 (*dispatch3C)(void *receiver, void *event); } ItemVTable;
struct Item { u8 type, kind, flags, field3; s32 field4; ItemVTable *vtable; };
typedef struct { u8 pad0[0x98]; short delta98, pad9A; void *(*inventory9C)(void *); } UnitVTable;
struct Unit { u8 pad0[0x1E]; u8 flags; u8 pad1F[5]; UnitVTable *vtable; u8 pad28[0xDC]; s32 field104; };
typedef struct { Unit *source; u8 pad4[0xC]; Item *item; } Object;
typedef struct { s32 index; void *container; s32 reverse; Item *current; } Iterator;
typedef struct RandomState RandomState;
extern u32 D_8013960C;
extern RandomState D_80147620;
/* Both source and target are caller-supplied Unit pointers. */
extern void func_800C4C54(Object *object, Unit *source, u8 *direction, Unit *target);
extern char *func_800AC9D8(void *item);
extern s32 func_800CD4C4(void *container, void *item);
extern s32 func_8010BF6C(u8 *item, Unit *unit, s32 check);
/* Original outputs are two inventory item addresses, dereferenced at +0x8 here. */
extern u8 func_800E8B10(Unit *unit, Item **items);
extern void func_800AE518(void *item, void *unit, s32 enable, s32 check);
extern char *func_800A3B20(Unit *unit);
extern void func_800498E4(s32 id, ...);
extern s32 func_800A08D8(s32 mode, s32 key, s32 selection);
extern s32 func_80049CB4(s32 id, ...);
extern s32 func_800CD538(void *container, Item *item);
extern Iterator *func_800CEB20(Iterator *iterator, void *container);
extern s32 func_800CEBA0(Iterator *iterator);
extern Item *func_800CEC68(Iterator *iterator);
extern u8 func_800C57A0(void *rng);
extern s32 func_80114330(Item *target, Item *source);
extern char *func_800AE674(void *item);
extern char *func_80114BCC(Item *item, char *message, s32 with_extra);
/* ODD_C: named field/message helpers preserve the original switch and call temporaries. */
static __inline__ s32 active(Unit *unit) { return (unit->flags >> 2) & 1; }
static __inline__ s32 locked(Item *item) { return item->vtable->locked14((char *)item + item->vtable->delta10); }
static __inline__ void initialize_message(Message *message, Unit *source, u8 *direction, Unit *target) {
    message->kind = 0x12;
    message->source = source;
    message->target = target;
    message->direction = *direction;
    message->repeat = source;
}
void func_80101FF0(Object *object, Unit *source, u8 *direction, Unit *unit) {
    char *name;
    s32 eligible;
    if (source != object->source) {
        func_800C4C54(object, source, direction, unit);
        return;
    }
    name = func_800AC9D8(object->item);
    eligible = 0;
    if (active(unit)) eligible = unit->field104 == 0;
    if (eligible) {
        switch (object->item->type) {
        case 3:
        case 4:
        case 6: {
            void *container = unit->vtable->inventory9C((char *)unit + unit->vtable->delta98);
            s32 present = 0;
            if (locked(object->item)) present = func_800CD4C4(container, object->item) != 0;
            if (present) {
                D_8013960C <<= 1;
                /* Each kind equips the item at the end of its own branch; the
                   identical tails are cross-jumped into the one ROM call site. */
                if (object->item->type != 6) {
                    if (func_8010BF6C((u8 *)object->item, unit, 1) == 0) goto fail;
                    func_800AE518(object->item, unit, 1, 0);
                } else {
                    Item *equipped[2];
                    if (func_800E8B10(unit, equipped) >= 2) {
                        Item *other = equipped[0];
                        if (locked(other)) {
                            other = equipped[1];
                            if (locked(other)) {
                            /* Shared failure exit: both a rejected non-6 item
                             * and two locked equipped items restore the state
                             * and leave the switch without equipping. */
                            fail:
                                D_8013960C >>= 1;
                                break;
                            }
                        }
                        func_800AE518(other, unit, 0, 0);
                    }
                    func_800AE518(object->item, unit, 1, 0);
                }
                D_8013960C >>= 1;
                func_800498E4(0x5C, func_800A3B20(unit), name);
                func_800A08D8(1, -1, 0);
                func_80049CB4(0x12B);
                func_800498E4(0x2E, name);
                func_800CD538(container, object->item);
                return;
            }
            break;
        }
        }
        {
            Iterator iterator;
            D_8013960C <<= 1;
            func_800CEB20(&iterator, unit->vtable->inventory9C((char *)unit + unit->vtable->delta98));
            while (func_800CEBA0(&iterator)) {
                Item *other = func_800CEC68(&iterator);
                s32 accepts = 0;
                if (other->type == 9) accepts = other->vtable->query1C((char *)other + other->vtable->delta18, 0xE) != 0;
                if (accepts) {
                    s32 transfer = 0;
                    if (func_800C57A0(&D_80147620) & 1) transfer = func_80114330(other, object->item) != 0;
                    if (transfer) {
                        Message message;
                        char *other_name, *item_name;
                        D_8013960C >>= 1;
                        other_name = func_800AE674(other);
                        item_name = func_800AE674(object->item);
                        func_800498E4(0x98, item_name, func_80114BCC(other, other_name, 0));
                        func_800A08D8(1, -1, 0);
                        message.target = object->item;
                        message.kind = 0xA;
                        message.source = source;
                        other->vtable->dispatch3C((char *)other + other->vtable->delta38, &message);
                        return;
                    }
                }
            }
            D_8013960C >>= 1;
        }
    }
    {
        Message message;
        initialize_message(&message, source, direction, unit);
        object->item->vtable->dispatch3C((char *)object->item + object->item->vtable->delta38, &message);
    }
}
