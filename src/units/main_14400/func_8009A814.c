#include "common.h"
typedef unsigned char u8;
typedef struct MenuBase { char pad0[0x4C]; const void *vtbl; } MenuBase;
/* Five consecutive objects have this complete 0x40C-byte extent. */
typedef struct MenuPanel {
    MenuBase base;
    char pad50[0xC8 - 0x50];
    MenuBase second;
    char pad118[0x128 - 0x118];
    char unk128[0x194 - 0x128];
    const void *unk194;
    char pad198[0x2A0 - 0x198];
    const void *unk2A0;
    char pad2A4[0x2CC - 0x2A4];
    s32 field_2CC, field_2D0;
    char pad2D4[0x40C - 0x2D4];
} MenuPanel;
typedef MenuPanel Actor8009A814;
typedef struct { s32 field_0; Actor8009A814 *actor; s32 kind; } Entry8009A814;
typedef struct { u8 pad0[0x50]; Entry8009A814 *entries; } Obj8009A814;
extern Actor8009A814 D_801408EC;
static inline void clear_selection(MenuPanel *panel) { panel->field_2CC = 0; }
Actor8009A814 *func_8009A814(Obj8009A814 *obj, s32 index) {
    Actor8009A814 *actor = obj->entries[index].actor;
    Entry8009A814 *entry = &obj->entries[index];
    if (actor == &D_801408EC) {
        switch (entry->kind) {
            case 0x16: case 0x19: clear_selection(&D_801408EC); break;
            case 0x15: actor->field_2CC = 1; break;
        }
    }
    return actor;
}
