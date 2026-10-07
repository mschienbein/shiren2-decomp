#include "common.h"

typedef struct { s32 x; s32 y; } Pair;
extern Pair *func_800A256C(Pair *, Pair *, Pair *);
extern void *func_800A21F0(void *, Pair *);

void *func_800A27A4(void *out_direction, void *from, void *to) {
    Pair value;
    func_800A256C(&value, to, from);
    func_800A21F0(out_direction, &value);
    return out_direction;
}
