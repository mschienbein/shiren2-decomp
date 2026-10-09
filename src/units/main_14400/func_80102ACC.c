#include "common.h"
typedef unsigned char u8;
typedef struct { s32 x, y; } Pair;
typedef struct { u8 value; } Dir;
typedef struct { Pair field0; u8 field8; u8 pad9[0x4B]; u8 field54; u8 pad55[3]; void *field58; s32 field5C, field60; Pair field64; u8 pad6C[0x34]; Pair fieldA0; } Object;
extern u8 func_800A6420(Object *, void *);
extern void func_800F06E4(Object *);
extern void *func_800A65E4(u8 *, Object *, void *);
extern void func_800A665C(Object *, u8 *);
extern s32 func_801029B0(Object *, Pair *);
extern Pair *func_80102884(Pair *, Object *);
extern void *func_800A2594(Pair *, Pair *, Dir);
extern u32 func_800B1C6C(Pair *);
extern void *func_800A7DEC(Object *);
extern s32 func_800A46BC(Object *, void *, u8 *);
extern void func_800A2F80(u8 *, s32);
extern s32 func_800E7104(Object *);
static inline void copy(Pair *dest, const Pair *source) {
    dest->x = source->x;
    dest->y = source->y;
}
static inline s32 nonzero(Pair *position) { return position->y | position->x; }
static inline void *target_for(Object *obj) { return obj->field58; }
static inline s32 can_move(Object *obj, u8 *direction) {
    return func_800A46BC(obj, func_800A7DEC(obj), direction) != 0;
}
s32 func_80102ACC(Object *obj) {
    Pair origin, position, temp;
    u8 new_direction, direction;
    s32 mode, i;
    void *target;
    target = target_for(obj);
    copy(&origin, &obj->field0);
    direction = obj->field8;
    mode = func_800A6420(obj, target);
    switch (mode) {
    case 0:
        func_800F06E4(obj);
        return 0;
    case 1:
    case 2:
        func_800A65E4(&new_direction, obj, target);
        direction = new_direction;
        break;
    default:
        {
            s32 refresh = 0;
            copy(&position, &obj->fieldA0);
            if (!nonzero(&position) || !func_801029B0(obj, &position)) refresh = 1;
            if (refresh) {
                func_80102884(&temp, obj);
                position = temp;
                obj->fieldA0 = position;
            }
            if (nonzero(&position)) obj->field64 = position;
        }
        break;
    }
    for (i = 0;; i++) {
        s32 success;
        Dir step;
        if (i >= 8) break;
        step.value = direction;
        func_800A2594(&position, &origin, step);
        success = 0;
        if (!(func_800B1C6C(&position) & 0x2000) && func_801029B0(obj, &position)) {
            success = can_move(obj, &direction);
        }
        if (success) {
            func_800A665C(obj, &direction);
            obj->field58 = 0;
            obj->field54 |= 4;
            return 0;
        }
        func_800A2F80(&direction, 1);
    }
    return func_800E7104(obj);
}
