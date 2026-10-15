#include "map_unit_iterator.h"
#include "item_records.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;


typedef struct {
    s32 unk0;
    void *target;
    u8 dir;
    u8 pad9[0x10 - 9];
} Side800F4F8C;

typedef struct {
    ItemMessage base;
    s32 fromId;
    void *fromTarget;
    u8 fromDir;
    u8 pad0D[0x18 - 0xD];
    s32 unk18;
    s32 toId;
    u8 toDir;
    u8 pad21[7];
} Msg800F4F8C;


typedef struct {
    s32 type;
    s32 id;
    Item *item;
} Event800F4F8C;

typedef struct {
    u8 pad0[8];
    u8 dir;
    u8 pad9[0x1C - 9];
    u16 flags1C;
} Unit800F4F8C;

s32 func_80049CB4(s32 id, ...);
char *func_800AC9D8(Item *item);
char *func_800A3B20(void *self);
void func_800497F0(s32 id, ...);
void func_800D3650(Item *item);
s32 func_800F4A70(void *self, Event800F4F8C *ev);

static inline s32 isHidden(Unit800F4F8C *unit) {
    return unit->flags1C & 1;
}

s32 func_800F4F8C(void *self, Event800F4F8C *ev) {
    if (ev->type == 0xF) {
        Item *item = ev->item;
        s32 match = 0;

        if (item->field00 == 1 || item->field00 == 7 || item->field01 == 0x17) {
            match = 1;
        }
        if (match) {
            s32 a = func_80049CB4(0xB6, self);
            char *b = func_800AC9D8(item);
            MapUnitIterator it;

            func_800497F0(0x63, a, b, func_800A3B20(self));
            func_800A915C(&it, self);
            while (func_800A9284(&it, 0x7C)) {
                Unit800F4F8C *unit = func_800A942C(&it);

                s32 active = isHidden(unit) != 1;

                if (active) {
                    Msg800F4F8C msg;
                    s32 id = ev->id;
                    u8 dir = (unit->dir + 4) & 7;

                    msg.base.kind = 0x13;
                    msg.fromTarget = unit;
                    msg.unk18 = 0;
                    msg.toDir = dir;
                    msg.fromId = id;
                    msg.fromDir = dir;
                    msg.toId = id;
                    item->field08->handle3C((u8 *)item + item->field08->delta38, &msg.base);
                }
            }
            func_800D3650(item);
            if (item != 0) {
                item->field08->method0C((u8 *)item + item->field08->delta08, 3);
            }
            return 1;
        }
    }
    return func_800F4A70(self, ev);
}
