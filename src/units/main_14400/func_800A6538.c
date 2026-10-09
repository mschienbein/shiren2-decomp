#include "common.h"
typedef unsigned char u8;
typedef struct Pair { s32 x; s32 y; } Pair;
typedef struct { Pair pos; u8 direction; } Obj;
s32 func_800A251C(Pair *x, Pair *y);
void *func_800A27A4(void *out_direction, void *from, void *to);
static inline void copy_position(Pair *out, Pair *from) { out->x = from->x; out->y = from->y; }
void *func_800A6538(void *out_direction, void *obj, void *target) {
    u8 direction;
    if (func_800A251C(target, obj)) {
        direction = ((Obj *)obj)->direction;
    } else {
        Pair copy;
        u8 toward;
        copy_position(&copy, target);
        func_800A27A4(&toward, obj, &copy);
        direction = toward;
    }
    *(u8 *)out_direction = direction;
    return out_direction;
}
