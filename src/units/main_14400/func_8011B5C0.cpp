#include "common.h"
#include "collection_iterator.h"

typedef unsigned char u8;
typedef short s16;
typedef struct { s32 kind; u8 pad_04[0x14]; void *field_18; u8 pad_1C[4]; } Event_8011B5C0;
typedef struct { u8 pad_00[0x10]; s16 delta_10; s16 pad_12; s32 (*test)(void *); u8 pad_18[0x20]; s16 delta_38; s16 pad_3A; s32 (*event)(void *, void *); } ItemVTable_8011B5C0;
typedef struct Ent_8011B5C0 { u8 pad_00[2]; u8 field_02; u8 pad_03[5]; ItemVTable_8011B5C0 *field_08; } Ent_8011B5C0;
typedef struct { u8 pad_00[0x98]; s16 delta_98; s16 pad_9A; void *(*inventory)(void *); } UnitVTable_8011B5C0;
typedef struct { u8 pad_00[0x24]; UnitVTable_8011B5C0 *field_24; } Unit_8011B5C0;
extern "C" {
extern s32 func_80049CB4(s32 id, ...);
extern char *func_800A3B20(Unit_8011B5C0 *u);
extern void func_800498E4(s32 id, ...);
}


extern "C" s32 func_8011B5C0(void *object /* unused supplied receiver */, Unit_8011B5C0 *unit) {
    Event_8011B5C0 event;
    s32 changed = 0;
    void *inventory;
    event.kind = 0x19;
    event.field_18 = 0;
    inventory = unit->field_24->inventory((u8 *)unit + unit->field_24->delta_98);
    if (inventory) {
        /* The shared record is initialized by the real provider in this body, as in the C
           callers. ROM 0x8011B604-0x8011B614 keeps &iterator in s0 across the CEB20 call and
           copies it to the cursor; an inline wrapper constructor would hand the provider a
           rematerialized frame address instead (260 B, 49 words). */
        CollectionIterator iterator;
        func_800CEB20(&iterator, inventory);
        CollectionIterator *cursor = &iterator;
        while (func_800CEBA0(cursor)) {
            Ent_8011B5C0 *item = static_cast<Ent_8011B5C0 *>(func_800CEC68(cursor));
            s32 eligible = 0;
            if (item->field_02 & 4) {
                eligible = item->field_08->test((u8 *)item + item->field_08->delta_10) != 0;
            }
            if (eligible) {
                ItemVTable_8011B5C0 *table = item->field_08;
                table->event((u8 *)item + table->delta_38, &event);
                changed = 1;
            }
        }
    }
    if (changed) {
        func_80049CB4(0x43, unit);
        func_800498E4(0xD1, func_800A3B20(unit));
    }
    return changed;
}
