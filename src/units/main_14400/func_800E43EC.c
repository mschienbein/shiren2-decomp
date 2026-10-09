#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    s32 x;
    s32 y;
} Position;

typedef struct {
    Position pos;   /* 0x00 */
    u8 pad08[0x1F - 0x08];
    u8 field_1F;    /* 0x1F */
    u8 pad20[0x75 - 0x20];
    u8 field_75;    /* 0x75 */
} Unit;

s32 func_80049CB4(s32 id, ...);
void func_800A7C1C(void *obj);

/* ODD_C: the by-address copy keeps &pos in v1; direct stores or a struct
 * copy fold the address into sp and shorten the object from 104 to 100 bytes. */
static inline void getPosition(Unit *unit, Position *out) {
    out->x = unit->pos.x;
    out->y = unit->pos.y;
}

/* Announces the unit's position (message 0x109), then applies its new kind and state.
   [INFERENCE] The u16 kind is inferred from the unmasked late register copy,
   not a halfword store; the ROM stores only its low byte. Every caller passes
   a zero-extended byte, while a u8 parameter shortens the object to 100 bytes. */
void func_800E43EC(Unit *unit, u16 kind, u8 state) {
    Position pos;

    getPosition(unit, &pos);
    func_80049CB4(0x109, &pos);
    unit->field_1F = kind;
    unit->field_75 = state;
    func_800A7C1C(unit);
}
