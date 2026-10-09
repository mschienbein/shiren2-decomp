#include "common.h"

typedef struct { unsigned char value; } Dir;
typedef struct { s32 x, y; } Pair;
typedef struct Object Object;
extern void *func_800A2594(Pair *out, void *arg, Dir cell);
extern void *func_800B49B8(Pair *p);
extern s32 func_800A4CC4(void *p, void *v, void *a);
extern s32 func_800B48C0(Pair *p, Object *object);

s32 func_800A4E30(Pair *position, Dir *direction) {
    Pair other;
    Object *first;
    Object *second;
    s32 result;
    func_800A2594(&other, position, *direction);
    first = func_800B49B8(position);
    second = func_800B49B8(&other);
    result = func_800A4CC4(position, position, direction);
    func_800B48C0(position, first);
    func_800B48C0(&other, second);
    return result;
}
