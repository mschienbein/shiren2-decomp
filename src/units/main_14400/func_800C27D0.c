#include "common.h"
typedef unsigned char u8;
typedef short s16;
typedef struct ShirenDirection { signed char value; } ShirenDirection;
typedef struct { s32 x, y; } Pair;
typedef struct {
    Pair origin;
    u8 direction;
    u8 limit;
    u8 field_0A;
    u8 mode;
    s16 field_0C;
    s16 field_0E;
    s32 field_10;
    s16 field_14;
} Iterator;

void func_800C27D0(void *iterator, void *origin, ShirenDirection direction, unsigned char limit, unsigned char mode) {
    Iterator *it = (Iterator *)iterator;
    it->origin = *(Pair *)origin;
    it->direction = direction.value;
    it->limit = limit;
    it->field_0A = 0;
    it->field_14 = 0;
    it->field_0E = 0;
    it->field_0C = 0;
    it->field_10 = 1;
    it->mode = mode;
}
