#include "common.h"
typedef unsigned char u8;
typedef struct { signed char value; } Direction;
typedef struct { s32 x, y; } Position;
typedef struct { u8 pad0[0x38]; s32 field38; } Object;
extern s32 func_801368B4(Position *position, s32 mask);
extern void func_8011205C(Object *object, Position *position, Direction direction);
extern s32 func_800B43BC(void *cell, s32 enabled, u8 direction);
extern s32 func_800B4888(Position *cell);
extern s32 func_80112084(Object *object, s32 kind, Position *position, Direction direction);
static __inline__ u8 direction_value(Direction *direction) {
    return direction->value;
}
static __inline__ s32 is_even(Direction *direction) {
    return (direction_value(direction) ^ 1) & 1;
}
static __inline__ void copy_position(Position *out, Position *in) {
    out->x = in->x;
    out->y = in->y;
}
s32 func_8011F720(Object *object, s32 kind, Position *position, Direction direction) {
    Position copy;
    if (kind == 1) {
        s32 allowed = 0;
        if (is_even(&direction)) {
            s32 clear = func_801368B4(position, 0x4000) && !func_801368B4(position, 0x8020);
            if (clear) allowed = 1;
        }
        if (allowed) {
            if (object->field38 == 0) {
                func_8011205C(object, position, direction);
                object->field38 = 1;
            }
            func_800B43BC(position, 1, (u8)direction.value);
            return !func_800B4888(position);
        }
    } else if (object->field38 != 0) return 0;
    copy_position(&copy, position);
    return func_80112084(object, kind, &copy, direction);
}
