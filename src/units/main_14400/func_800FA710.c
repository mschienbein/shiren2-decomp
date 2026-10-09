#include "common.h"

typedef struct Object Object;
typedef struct Position Position;
extern s32 func_800F17A8(Object *obj, Position *position, s32 mode);
extern void *func_800B4D80(Position *p);
extern s32 func_800FA67C(Object *obj, void *other);

s32 func_800FA710(Object *obj, Position *position) {
    if (func_800F17A8(obj, position, 0)) {
        return func_800FA67C(obj, func_800B4D80(position));
    }
    return 0;
}
