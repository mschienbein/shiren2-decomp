#include "common.h"

typedef unsigned char u8;
typedef struct { s32 x; s32 y; } Position;
typedef struct { u8 value; } Dir;
typedef struct { Position position; u8 field08; u8 field09; u8 pad0A[0x4E]; void *target58; } Object;
extern u8 func_800A6420(void *object, void *target);
extern void func_800F06E4(void *object);
extern s32 func_800A65B8(void *object, void *target);
extern void *func_800A65E4(Dir *direction, void *object, void *target);
extern s32 func_800A46BC(void *object, Position *position, Dir *direction);
extern s32 func_800F1024(void *object);
extern s32 func_800E7104(void *object);
extern s32 func_800A50AC(void *object);

s32 func_801053CC(Object *object)
{
    Position position;
    Dir direction;
    Position *pos;
    s32 mode = object->field09 & 0xF;
    s32 state;
    s32 handled;
    void *target;
    if (mode != 1)
        return 0;
    target = object->target58;
    state = func_800A6420(object, target);
    switch (state) {
    case 0:
        func_800F06E4(object);
        return 0;
    case 1:
    case 2:
        handled = 0;
        position.x = object->position.x;
        pos = &position;
        pos->y = object->position.y;
        if (func_800A65B8(object, target) == mode) {
            func_800A65E4(&direction, object, target);
            if (func_800A46BC(object, pos, &direction))
                handled = func_800F1024(object) != 0;
        }
        if (handled) {
            func_800F06E4(object);
            object->target58 = 0;
            return 0;
        }
        return func_800E7104(object);
    default:
        return func_800A50AC(object);
    }
}
