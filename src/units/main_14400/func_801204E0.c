#include "common.h"
#include "item_records.h"
typedef short s16;
typedef unsigned char u8;
typedef struct { s32 unk0, unk4; } Pair;
typedef struct Unit Unit;
typedef struct {
    char unk0[8]; s16 unk8, unkA; void (*unkC)(void *, s32);
    s16 unk10, unk12; s32 (*unk14)(void *);
    char unk18[8]; s16 unk20, unk22; s32 (*unk24)(void *);
    char unk28[0x10]; s16 unk38, unk3A; void (*unk3C)(void);
    char unk40[0x58]; s16 unk98, unk9A; void *(*unk9C)(void *);
} Dispatch;
/* A unit named by a message: map position words at +0x00/+0x04, method table at +0x24. */
struct Unit { Pair position; u8 pad_08[0x1C]; Dispatch *table; };
/* +0x08 is the message's object: an Item for kind 12, a Unit for kinds 18/19. */
typedef struct { ItemMessage base; Unit *unk4; void *unk8; u8 unkC; char unkD[0xB]; s32 unk18; Unit *unk1C; } Event;
extern u8 D_80147620[];
extern s32 func_800CD4C4(void *, Item *);
extern s32 func_800CD538(void *, Item *);
extern void func_800497F0(s32, ...);
extern void func_800498E4(s32, ...);
extern s32 func_80049CB4(s32, ...);
extern s32 func_800C5844(void *, u8, u8);
extern void func_800A7B18(Unit *, Unit *, s32, s32);
extern s32 func_800A08D8(s32, s32, s32);
extern void func_800D3650(Item *);
extern s32 func_80114E28(Item *, Event *);
static inline Pair *copy_pair(Pair *dest, Pair *source) { dest->unk0 = source->unk0; dest->unk4 = source->unk4; return dest; }
/* ODD_C: collection accessors take the collection address as an inline argument; GCC copies
   that argument, so the loop keeps its own hoisted copy of the collection pointer as in the ROM. */
static inline u32 collection_count(ItemList *collection) {
    const ItemListMethods *dispatch = collection->methods;
    return dispatch->count24((char *)collection + dispatch->delta20);
}
static inline Item *collection_item(ItemList *collection, u32 index) {
    const ItemListMethods *dispatch = collection->methods;
    return dispatch->get3C((char *)collection + dispatch->delta38, index);
}
s32 func_801204E0(Item *arg0, Event *arg1) {
    Pair position;
    Event event;
    switch (arg1->base.kind) {
    case 12: {
        Unit *source = arg1->unk4;
        Dispatch *dispatch = source->table;
        Item *target = arg1->unk8;
        s32 r = func_800CD4C4(dispatch->unk9C((char *)source + dispatch->unk98), target);
        char *name;
        r ^= 1;
        if (r) { func_800498E4(0x9D); return 0; }
        func_80049CB4(0x3C, source);
        name = func_800AE674(arg0);
        func_800498E4(0x9E, name, func_800AE674(target));
        dispatch = source->table;
        func_800CD538(dispatch->unk9C((char *)source + dispatch->unk98), target);
        return 1;
    }
    case 18: {
        Dispatch *dispatch;
        Unit *target = arg1->unk8;
        u32 count = collection_count(&arg0->payload.pot.list);
        if (arg0->payload.pot.field28 && count) {
            Pair *current = copy_pair(&position, &target->position);
            Unit *source;
            s32 message;
            s32 r;
            func_80049CB4(0x1131);
            func_80049CB4(6);
            message = func_80049CB4(0xFE, current);
            func_80049CB4(7);
            source = arg1->unk4;
            func_80049CB4(6);
            func_800A7B18(target, source, func_800C5844(D_80147620, 1, 3) & 0xFF, 0x21);
            func_80049CB4(7);
            {
                Dispatch *table = target->table;
                r = table->unk14((char *)target + table->unk10);
            }
            r ^= 1;
            if (r) {
                u32 i;
                func_800497F0(0xC3, message, func_800AE674(arg0));
                if (func_800A08D8(1, message, 0)) func_80049CB4(0x12A);
                i = 0;
                event.base.kind = 0x13;
                event.unk4 = source;
                event.unk8 = target;
                event.unkC = arg1->unkC;
                event.unk18 = 0;
                event.unk1C = source;
                for (;;) {
                    s32 in_range = i < count;
                    Item *item;
                    if (!in_range) break;
                    item = collection_item(&arg0->payload.pot.list, i);
                    dispatch = target->table;
                    if (dispatch->unk14((char *)target + dispatch->unk10)) break;
                    func_800497F0(0x62, message, func_800AE674(item));
                    {
                        ItemMethods *table = item->field08;
                        table->handle3C((char *)item + table->delta38, &event.base);
                    }
                    i++;
                    func_800A08D8(1, message, 0);
                }
            }
            func_800D3650(arg0);
            if (arg0) {
                ItemMethods *table = arg0->field08;
                table->method0C((char *)arg0 + table->delta08, 3);
            }
            return 1;
        }
        break;
    }
    }
    return func_80114E28(arg0, arg1);
}
