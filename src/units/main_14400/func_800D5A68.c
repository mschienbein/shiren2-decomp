#include "common.h"
typedef unsigned char u8;
typedef struct { s32 x, y; } Position;
typedef struct { Position position; u8 direction; } Object;
extern Object *D_80147FE0;
u8 D_80147FF0 = 0;
extern u8 D_80147FFC;
extern Position D_80147FF4;
extern void func_800A665C(Object *object, u8 *direction);
extern s32 func_800A251C(Position *a, Position *b);
extern s32 func_800D5670(void *position, u8 *direction);
extern s32 func_800D5730(Position *position, u8 *direction);
extern s32 func_800D5968(Position *position);
extern void *func_800A7DEC(void *object);
extern s32 func_800A4CC4(void *object, void *value, void *direction);
extern void *func_800A6CC0(void *position, void *object);
extern s32 func_800FCF3C(Position *position, s32 mode);
extern char *func_800A7DE4(Object *object);
extern s32 func_800A4EFC(void *object, void *value);
static inline Position *copy_position(Position *out, Position *in) { out->x=in->x; out->y=in->y; return out; }
static inline s32 invert(s32 value) { return value ^ 1; }
s32 func_800D5A68(void) {
    Position position, next;
    Object *object;
    copy_position(&position, &D_80147FE0->position);
    if (D_80147FE0->direction != D_80147FF0) func_800A665C(D_80147FE0, &D_80147FF0);
    if (invert(func_800A251C(&D_80147FF4, &position))) return 1;
    if (D_80147FFC) {
        if (invert(func_800D5670(&position, &D_80147FF0))) return 1;
        if (invert(func_800D5730(&position, &D_80147FF0))) return 1;
        if (invert(func_800D5968(&position))) return 1;
    } else {
        D_80147FFC = 1;
        object = D_80147FE0;
        if (func_800A4CC4(object, func_800A7DEC(object), &D_80147FF0)) {
            func_800A6CC0(&next, D_80147FE0);
            if (func_800FCF3C(&next, 0)) return 0;
        }
    }
    object = D_80147FE0;
    if (func_800A4EFC(object, func_800A7DE4(object))) {
        D_80147FF4 = D_80147FE0->position;
        return 2;
    }
    return 1;
}
