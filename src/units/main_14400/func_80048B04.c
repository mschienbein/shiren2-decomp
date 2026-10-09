#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;

typedef unsigned char u8;
typedef struct { s32 x, y; } Pair;
typedef struct { Pair first, last; } Rect;
typedef struct { u8 kind, flags; } AreaRule;
typedef struct { Rect *area; s32 kind, field8, active; } ActiveArea;

extern AreaRule D_801390C0[];
extern s32 D_8013968C;
extern s32 D_80139690;
extern Pair *D_801476B8;

extern ActiveArea D_80143434;

Pair *func_800C5F60(void);
s32 func_800A251C(Pair *x, Pair *y);
s32 func_80041548(s32 y, s32 x);
Rect *func_800B310C(Rect *value, Pair *object);
/* These two return their Rect by value through the hidden result pointer (no v0 result at call sites). */
Rect func_800B2EDC(Pair *object);
Rect func_800B3024(Pair *object);
s32 func_800A31C8(Rect *a, Pair *b);
s32 func_800A23E8(Pair *origin, Pair *vec);
s32 func_800627C4(void);
s32 func_800C965C(void);
void *func_800B4928(Pair *pos);

static inline void pair_copy(Pair *dst, Pair *src) {
    dst->x = src->x;
    dst->y = src->y;
}

s32 func_80048B04(Pair *pos) {
    s32 flags = D_801390C0[D_8013968C].flags;
    s32 allowed = 0;
    Pair occupied;
    Pair center;
    /* Area under test; its first corner is later reused as the distance target. */
    Rect area;
    union {
        struct { Pair target; Rect outer; } special;
        Rect outer;
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
                pair_copy(&area.first, pos);
                if (func_800A23E8(&center, &area.first) < 6) {
                    allowed = 1;
                }
            }
            if (allowed && (flags & 0x10)) {
                pair_copy(&area.first, pos);
                if (func_800A23E8(&center, &area.first) >= 6) {
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
