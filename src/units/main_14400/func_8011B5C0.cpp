#include "common.h"

typedef unsigned char u8;
typedef short s16;
inline void *operator new(unsigned int, void *p) { return p; }
typedef struct { s32 kind; u8 pad_04[0x14]; void *field_18; u8 pad_1C[4]; } Event;
typedef struct { u8 pad_00[0x10]; s16 delta_10; s16 pad_12; s32 (*test)(void *); u8 pad_18[0x20]; s16 delta_38; s16 pad_3A; s32 (*event)(void *, void *); } ItemVTable;
typedef struct Ent { u8 pad_00[2]; u8 field_02; u8 pad_03[5]; ItemVTable *field_08; } Ent;
typedef struct { s32 field_00; void *field_04; s32 field_08; } S;
extern "C" S *func_800CEB20(S *s, void *a);
struct Iter {
    S base;
    Ent *current;
    Iter() {}
    Iter(void *inventory) { func_800CEB20(&base, inventory); }
};
typedef struct { u8 pad_00[0x98]; s16 delta_98; s16 pad_9A; void *(*inventory)(void *); } UnitVTable;
typedef struct { u8 pad_00[0x24]; UnitVTable *field_24; } Unit;
extern "C" {
extern s32 func_800CEBA0(Iter *);
extern Ent *func_800CEC68(Iter *);
extern s32 func_80049CB4(s32 id, ...);
extern char *func_800A3B20(Unit *u);
extern void func_800498E4(s32 id, ...);
}


extern "C" s32 func_8011B5C0(void *object /* unused supplied receiver */, Unit *unit) {
    Event event;
    Iter iterator;
    s32 changed = 0;
    void *inventory;
    event.kind = 0x19;
    event.field_18 = 0;
    inventory = unit->field_24->inventory((u8 *)unit + unit->field_24->delta_98);
    if (inventory) {
        Iter *cursor = new (&iterator) Iter(inventory);
        while (func_800CEBA0(cursor)) {
            Ent *item = func_800CEC68(cursor);
            s32 eligible = 0;
            if (item->field_02 & 4) {
                eligible = item->field_08->test((u8 *)item + item->field_08->delta_10) != 0;
            }
            if (eligible) {
                ItemVTable *table = item->field_08;
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
