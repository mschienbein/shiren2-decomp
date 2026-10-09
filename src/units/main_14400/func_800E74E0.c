#include "common.h"

typedef struct {
    s32 x;
    s32 y;
} Pos;

typedef struct {
    Pos pos;
    unsigned char pad8[0x5C];
    Pos dir;
    unsigned char pad6C[0xA];
    unsigned char unk76;
} Obj;

extern unsigned char func_800A6420(Obj *obj, Obj *target);
extern s32 func_800A23E8(Pos *from, Pos *to);
extern s32 func_800A251C(Pos *a, Pos *b);
extern s32 func_800E62BC(Obj *obj, Pos *target, s32 range);
extern s32 func_800E65C0(Obj *obj, Pos *dir, s32 arg2);
extern s32 func_800E66EC(Obj *obj);

static inline void copyPos(Pos *dst, Pos *src) {
    dst->x = src->x;
    dst->y = src->y;
}

static inline s32 sameCell(Pos *a, Pos *b) {
    return func_800A251C(a, b);
}

s32 func_800E74E0(Obj *obj, Obj *other, s32 range) {
    Pos target;
    Pos dir;
    Pos cur;
    Pos tmp;
    unsigned char mode;
    s32 dist;
    unsigned char limit; /* callers pass the u8 range widened to s32 */

    copyPos(&target, &other->pos);
    copyPos(&dir, &obj->dir);
    copyPos(&cur, &obj->pos);
    limit = range;
    mode = func_800A6420(obj, other);
    if ((dir.y | dir.x) == 0) {
        dir = target;
    }
    copyPos(&tmp, &target);
    dist = func_800A23E8(&cur, &tmp);
    if (mode == 3 && dist >= 2 && dist <= limit) {
        if (func_800E62BC(obj, &target, limit * 2)) {
            mode = 1;
            obj->unk76 = 0;
        }
    } else if (dist == 1) {
        mode = 0;
    }
    switch (mode) {
        case 0:
            obj->dir = target;
            if ((sameCell(&target, &dir) ^ 1) != 0) {
                return func_800E65C0(obj, &dir, 1);
            }
            return 0;
        case 1:
        case 2:
            obj->dir = target;
            copyPos(&tmp, &dir);
            if (func_800A23E8(&target, &tmp) == 1) {
                if (func_800E65C0(obj, &dir, 1)) {
                    return 1;
                }
            } else {
                if (func_800E65C0(obj, &target, 1)) {
                    return 1;
                }
            }
            return func_800E66EC(obj);
        case 3:
            dir.x = 0;
            dir.y = 0;
            target.x = 0;
            target.y = 0;
            /* fallthrough */
        default:
            copyPos(&tmp, &dir);
            if (func_800A23E8(&target, &tmp) != 1 || (func_800E65C0(obj, &dir, 1) ^ 1) != 0) {
                if ((func_800E66EC(obj) ^ 1) != 0) {
                    return 0;
                }
            }
            copyPos(&tmp, &obj->pos);
            if (func_800A23E8(&target, &tmp) == 1) {
                obj->dir = target;
            }
            return 1;
    }
}
