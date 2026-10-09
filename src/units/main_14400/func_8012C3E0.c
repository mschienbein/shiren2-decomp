#include "common.h"

typedef unsigned char u8;
typedef struct Record_8012A7C4 Record_8012A7C4;

/* Eight-byte definition entry: sequence data pointer and default priority. */
typedef struct {
    u8 *data;
    s32 priority;
} Entry;
/* Same variable-extent model as func_8012C330: the bank's 0x18-byte header is
 * followed by a resource-dependent number of eight-byte entries in the same
 * backing bank (GNU trailing array). */
typedef struct {
    unsigned char pad0[0x10];
    Record_8012A7C4 *field_10;
    s32 field_14;
    Entry entries[0];
} Def;
/* Complete 0x13C-byte voice slot (stride of the D_801CA6E0 table); only the
 * stream cursor, priority and definition pointer are read here. */
typedef struct {
    unsigned char pad0[4];
    u8 *cursor_4;
    unsigned char pad8[0x40];
    s32 priority_48;
    unsigned char pad4C[0x2C];
    Def *def_78;
    unsigned char pad7C[0xC0];
} Slot;
extern s32 D_801CA6D4;
extern Slot *D_801CA6E0;
extern s32 func_8012C330(Slot *voice, Def *definition, s32 index, s32 value_a, s32 value_b, s32 priority);
s32 func_8012C3E0(Def *definition, s32 index, s32 value_a, s32 value_b, s32 priority) {
    Slot *slot, *best;
    s32 i, lowest;
    if (priority == -1) {
        priority = definition->entries[index].priority;
    }
    lowest = priority + 1;
    slot = D_801CA6E0;
    for (i = 4; i < D_801CA6D4; ++i, ++slot) {
        if (!slot->cursor_4) return func_8012C330(slot, definition, index, value_a, value_b, priority);
        if (slot->def_78 && slot->priority_48 < lowest) { lowest = slot->priority_48; best = slot; }
    }
    if (lowest < priority) return func_8012C330(best, definition, index, value_a, value_b, priority);
    return 0;
}
