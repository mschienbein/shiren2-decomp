#include "common.h"

typedef unsigned char u8;
typedef short s16;
/* Serialized data has a four-byte header followed by the EF74C byte-count payload. */
typedef struct { s32 marker; u8 payload[0]; } SaveData;
typedef struct { u8 pad_00[0xC]; s32 field_0C; u8 pad_10[8]; void *field_18; void *buffer; u32 position; u32 size; } Stream;
typedef struct { s32 kind; u8 fields[0x1C]; } Event;
typedef struct {
    u8 pad_00[8]; s16 delta_08; s16 pad_0A; void (*destroy)(void *, s32);
    u8 pad_10[0x18]; s16 delta_28; s16 pad_2A; void (*read)(void *, Stream *);
    u8 pad_30[8]; s16 delta_38; s16 pad_3A; s32 (*event)(void *, void *);
} ItemTable;
typedef struct { u8 kind; u8 pad_01[7]; ItemTable *field_08; } Item;
typedef struct { u8 pad_00[0x98]; s16 delta_98; s16 pad_9A; void *(*inventory)(void *); } UnitTable;
typedef struct { u8 pad_00[0xA]; u8 id; u8 pad_0B[0x19]; UnitTable *field_24; } Unit;
extern u8 D_801541F8[];
extern u8 D_80149F80[];
/* Original consumers dereference these two returned buffer addresses. */
extern SaveData *func_800EF72C(u8);
extern s32 func_800EF74C(u8);
extern void *func_800EF76C(u8);
extern s32 func_800EF78C(u8);
extern Stream *func_800CA030(Stream *);
extern void func_800CA600(void *, void *, u32);
extern void func_800CA668(Stream *, s32, void *);
extern void func_800EE9C0(Unit *, Stream *);
extern void func_800CD468(void *);
extern void *func_800AC244(u8);
extern s32 func_800CD5C0(void *, Item *);

void func_800EE718(Unit *unit, s32 mode) {
    u8 id = unit->id;
    SaveData *data = func_800EF72C(id);
    if (data->marker == -1) {
        s32 size = func_800EF74C(id) - 4;
        Stream stream;
        Stream *input = &stream;
        void *inventory;
        func_800CA030(input);
        input->field_18 = D_801541F8;
        func_800CA600(input, data->payload, size);
        if ((u8)mode == 1) {
            func_800EE9C0(unit, input);
        }
        inventory = unit->field_24->inventory((u8 *)unit + unit->field_24->delta_98);
        if (inventory) {
            void *buffer;
            s32 i;
            Event event;
            u8 count, item_id;
            func_800CD468(inventory);
            buffer = func_800EF76C(id);
            size = func_800EF78C(id);
            func_800CA600(input, buffer, size);
            func_800CA668(input, 1, &count);
            for (i = 0; ; i++) {
                Item *item;
                /* FAKEMATCH: the zero initialiser is dead. Setting message_kind twice makes
                 * loop.c treat the 0x1C load as a non-invariant user variable, so it stays
                 * inside the loop as in the ROM (otherwise it is hoisted into s2, 19 words).
                 * Tried without it: a block-local constant, inline init/send helpers, a direct
                 * event.kind store, a switch, and a per-iteration Event. */
                s32 message_kind = 0;
                if (i >= count) {
                    break;
                }
                func_800CA668(&stream, 1, &item_id);
                item = func_800AC244(item_id);
                if (item) {
                    if (item->kind == 9) {
                        message_kind = 0x1C;
                        event.kind = message_kind;
                        item->field_08->event((u8 *)item + item->field_08->delta_38, &event);
                    }
                    if (func_800CD5C0(inventory, item)) {
                        item->field_08->read((u8 *)item + item->field_08->delta_28, &stream);
                    } else {
                        item->field_08->destroy((u8 *)item + item->field_08->delta_08, 3);
                    }
                }
            }
        }
        stream.field_18 = D_80149F80;
    }
}
