#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    s32 x;
    s32 y;
} Position;

typedef struct Object {
    Position position;
    u8 pad08[0x1C - 0x08];
    u16 flags_1C;
    u8 pad1E[0x58 - 0x1E];
    struct Object *target_58;
} Object;

/* Step record filled by func_800A22B8 and consumed by func_800A665C. */
typedef struct {
    u8 bytes[8];
} Dir;

extern u8 func_800A6420(Object *obj, Object *target);
extern void func_800F06E4(Object *obj);
extern s32 func_800F1024(Object *object);
extern Dir *func_800A22B8(Dir *out, void *from, void *to);
extern void func_800A665C(Object *obj, u8 *value);
extern s32 func_800E7104(Object *unit);

static inline Object *target_of(Object *obj) {
    return obj->target_58;
}

/* Monster +0xA4 action slot (D_80159440 family): s32 (self). */
s32 func_80107A30(Object *obj) {
    Position pos;
    Position *p = &pos;
    Dir dir;
    Object *target;
    u8 result;

    pos.x = obj->position.x;
    p->y = obj->position.y;
    if (obj->flags_1C & 2) {
        target = target_of(obj);
        result = func_800A6420(obj, target);
        if (result == 0) {
            func_800F06E4(obj);
        } else if (func_800F1024(obj) != 0) {
            func_800F06E4(obj);
            obj->target_58 = 0;
        } else if (result != 3) {
            func_800A22B8(&dir, p, target);
            func_800A665C(obj, dir.bytes);
        }
        return 0;
    }
    return func_800E7104(obj);
}
