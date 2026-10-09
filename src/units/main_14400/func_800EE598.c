#include "common.h"

typedef unsigned char u8;
typedef short s16;

/* Serialized data has a four-byte header followed by the EF74C byte-count payload. */
typedef struct { s32 marker; u8 payload[0]; } SaveData;
typedef struct {
    u8 pad_00[0xC];
    s32 field_0C;
    u8 pad_10[8];
    void *field_18;
    void *buffer;
    u32 position;
    u32 size;
} Stream;
typedef struct {
    u8 pad_00[0x20];
    s16 delta_20;
    s16 pad_22;
    void (*write)(void *self, Stream *stream);
} ItemTable;
typedef struct { u8 kind; u8 id; u8 pad_02[6]; ItemTable *field_08; } Item;
/* Container slots: +0x24 func_800CE710 returns the s32 count, +0x3C func_800CE7A0
 * returns the item pointer (D_80154390/D_80154438/D_80154550). */
typedef struct {
    u8 pad_00[0x20];
    s16 delta_20;
    s16 pad_22;
    s32 (*count)(void *self);
    u8 pad_28[0x10];
    s16 delta_38;
    s16 pad_3A;
    Item *(*get)(void *self, u32 index);
} InventoryTable;
typedef struct { u8 pad_00[4]; InventoryTable *field_04; } Inventory;
typedef struct {
    u8 pad_00[0x98];
    s16 delta_98;
    s16 pad_9A;
    Inventory *(*inventory)(void *self);
} UnitTable;
typedef struct { u8 pad_00[0xA]; u8 id; u8 pad_0B[0x19]; UnitTable *field_24; } Unit;

extern u8 D_801541F8[];
extern u8 D_80149F80[];

/* Original consumers dereference these two returned buffer addresses. */
SaveData *func_800EF72C(u8 id);
s32 func_800EF74C(u8 id);
void *func_800EF76C(u8 index);
s32 func_800EF78C(u8 id);
Stream *func_800CA030(Stream *stream);
void func_800CA600(void *stream, void *buffer, u32 size);
void func_800CA610(Stream *stream, s32 size, void *data);
void func_800EE97C(Unit *unit, Stream *stream);

/* Saves the unit record and, if it carries an inventory, each item into its save slot. */
s32 func_800EE598(Unit *unit)
{
    u8 id = unit->id;
    SaveData *save = func_800EF72C(id);
    u8 *data;
    s32 size;
    Stream stream;
    Stream *output = &stream;
    Inventory *inventory;

    save->marker = -1;
    size = func_800EF74C(id) - 4;
    data = save->payload;
    func_800CA030(output);
    output->field_18 = D_801541F8;
    func_800CA600(output, data, size);
    func_800EE97C(unit, output);
    inventory = unit->field_24->inventory((u8 *)unit + unit->field_24->delta_98);
    if (inventory != 0) {
        void *buffer;
        s32 inventory_size;
        s32 i;
        u8 count;
        u8 item_id;

        buffer = func_800EF76C(id);
        inventory_size = func_800EF78C(id);
        func_800CA600(output, buffer, inventory_size);
        count = inventory->field_04->count((u8 *)inventory + inventory->field_04->delta_20);
        i = 0;
        func_800CA610(output, 1, &count);
        for (;;) {
            Item *item;

            if (i >= count) {
                break;
            }
            item = inventory->field_04->get((u8 *)inventory + inventory->field_04->delta_38, i++);
            item_id = item->id;
            func_800CA610(&stream, 1, &item_id);
            item->field_08->write((u8 *)item + item->field_08->delta_20, &stream);
        }
    }
    stream.field_18 = D_80149F80;
    return 1;
}
