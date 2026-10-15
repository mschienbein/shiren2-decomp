#include "common.h"
#include "map_rect.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05; unsigned short field_06; unsigned char mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;

typedef unsigned char u8;
typedef struct { u8 kind, flags; } AreaRule;
typedef struct { MapRect *area; s32 kind, field8, active; } ActiveArea;

extern AreaRule D_801390C0[];
extern s32 D_8013968C;
extern s32 D_80139690;
/* Position-prefix view of the foreign unit, not its complete object type. */
extern MapPos *D_801476B8;

extern ActiveArea D_80143434;

/* The returned foreign unit is read only through its two position words here. */
MapPos *func_800C5F60(void);
/* Scoped ABI ruling: scratch/omp/rulings/ABI-IDENTICAL-DECLARATIONS.md.
 * This by-value call supplies its hidden result pointer in a0 and receives it in v0,
 * exactly as B2EDC's explicit-out definition does. A by-value B2EDC definition copies
 * the result pointer at entry and differs in 7 words. Keep this view local to this TU. */
MapRect func_800B2EDC(void *object);
s32 func_800A251C(MapPos *x, MapPos *y);
s32 func_80041548(s32 y, s32 x);
/* Original-only ABI: a0 is the rectangle destination, a1 the position input,
 * and v0 returns the destination. This is not a recovered C++ signature. */
MapRect *func_800B310C(MapRect *value, MapPos *object);
s32 func_800A31C8(MapRect *bounds, MapPos *position);
s32 func_800A23E8(MapPos *origin, MapPos *vec);
s32 func_800627C4(void);
s32 func_800C965C(void);
void *func_800B4928(MapPos *pos);

static inline void pair_copy(MapPos *dst, MapPos *src) {
    dst->x = src->x;
    dst->y = src->y;
}

s32 func_80048B04(MapPos *pos) {
    s32 flags = D_801390C0[D_8013968C].flags;
    s32 allowed = 0;
    MapPos occupied;
    MapPos center;
    /* Area under test; its first corner is later reused as the distance target. */
    MapRect area;
    union {
        struct { MapPos target; MapRect outer; } special;
        MapRect outer;
    } more;

    if (flags == 0) {
        allowed = 1;
    } else {
        pair_copy(&occupied, D_801476B8);
        pair_copy(&center, func_800C5F60());
        if (func_800A251C(pos, &occupied)) {
            allowed = 1;
        } else if (flags & 4) {
            if (func_80041548(pos->y, pos->x)) {
                flags |= 2;
            }
            area = func_800B3024(&center);
            allowed = func_800A31C8(&area, pos);
            if (allowed) {
                pair_copy(&more.special.target, pos);
                if (func_800A23E8(&center, &more.special.target) >= 5) {
                    allowed = 0;
                }
            }
            if (!allowed && (flags & 2)) {
                func_800B310C(&more.special.outer, &center);
                allowed = func_800A31C8(&more.special.outer, pos);
            }
        } else {
            if (flags & 1) {
                s32 modeAllows = 0;
                if (func_800627C4() == 2 ||
                    (func_800627C4() == 3 && ((D_80142F18.flags >> 5) & 1))) {
                    modeAllows = 1;
                }
                if (modeAllows) {
                    flags |= 2;
                }
            }
            if (!(flags & 2)) {
                s32 active = 0;
                if (D_80143434.active || func_800C965C()) {
                    active = 1;
                }
                if (active && func_800B4928(pos)) {
                    flags |= 2;
                }
            }
            if (flags & 1) {
                if (flags & 0x20) {
                    area = func_800B2EDC(&center);
                } else {
                    area = func_800B3024(&center);
                }
                allowed = func_800A31C8(&area, pos);
                if (!allowed) {
                    func_800B310C(&more.outer, &center);
                    allowed = func_800A31C8(&more.outer, pos);
                }
            }
            if (!allowed && (flags & 2)) {
                pair_copy(&area.min, pos);
                if (func_800A23E8(&center, &area.min) < 6) {
                    allowed = 1;
                }
            }
            if (allowed && (flags & 0x10)) {
                pair_copy(&area.min, pos);
                if (func_800A23E8(&center, &area.min) >= 6) {
                    allowed = 0;
                }
            }
        }
    }
    D_80139690 = allowed;
    if (flags & 8) {
        allowed = 1;
    }
    return allowed;
}
