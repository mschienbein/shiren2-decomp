#include "common.h"
typedef unsigned char u8;
typedef struct { u8 direction; } Direction;
static inline u8 direction_value(const Direction *direction) {
    return direction->direction;
}
s32 func_800A2FA8(const Direction *origin, Direction target) {
    return (direction_value(&target) - direction_value(origin)) & 7;
}
