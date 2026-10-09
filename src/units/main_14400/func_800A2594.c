#include "common.h"
typedef unsigned char u8;
typedef struct { u8 value; } Dir;
typedef struct { s32 x, y; } Vec2_800A2544;
typedef Vec2_800A2544 Tmp800A46BC;
extern Vec2_800A2544 D_80142940[];
Vec2_800A2544 *func_800A2544(Vec2_800A2544 *out, Vec2_800A2544 *a, Vec2_800A2544 *b);
static inline Vec2_800A2544 *step(Dir *cell) {
    return &D_80142940[cell->value];
}
void *func_800A2594(Tmp800A46BC *out, void *arg, Dir cell) {
    func_800A2544(out, arg, step(&cell));
    return out;
}
