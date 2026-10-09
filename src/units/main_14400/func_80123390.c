#include "common.h"

typedef unsigned char u8;
typedef struct { s32 x; s32 y; } Position;
typedef struct { u8 pad_00[8]; u8 field_08; } Object;
typedef struct Obj Obj;
extern void *func_800A39C0(Position *, Object *, unsigned char *, s32);
/* All five operands are pointers; the callee retains unused direction/target slots. */
extern void func_8010E70C(Obj *obj, void *unit, u8 *direction, void *target, void *position);

void func_80123390(Obj *object, Object *unit) {
    Position position;
    u8 direction;
    direction = unit->field_08;
    func_800A39C0(&position, unit, &direction, 2);
    func_8010E70C(object, unit, &direction, unit, &position);
}
