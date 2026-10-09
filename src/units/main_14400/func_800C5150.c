#include "common.h"

typedef unsigned char u8;
typedef signed char s8;

typedef struct ShirenDirection {
    s8 value;
} ShirenDirection;

typedef struct Position {
    s32 words[2];
} Position;

/* func_800C25D0 fills 0x00..0x13; this constructor adds the cursor fields. */
typedef struct Iterator {
    Position origin_00;
    u8 direction_08;
    s32 field_0C;
    s32 field_10;
    Position cursor_14;
    s32 field_1C;
    u32 limit_20;
} Iterator;

void func_800C25D0(Iterator *obj, Position *pair, u8 *byte, s32 value);

void *func_800C5150(Iterator *it, Position *pos, ShirenDirection dir, s32 value, u32 limit) {
    func_800C25D0(it, pos, (u8 *)&dir, value);
    it->cursor_14 = *pos;
    it->field_1C = 1;
    it->limit_20 = limit;
    return it;
}
