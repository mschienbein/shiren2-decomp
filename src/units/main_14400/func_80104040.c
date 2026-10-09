#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x, y; } Pair;
typedef struct { Pair minimum, maximum; } Rect;
typedef struct { short adjust; u16 reserved; s32 (*call)(void *); } Entry;
typedef struct { u32 pad0 : 8; u32 bit23 : 1; u32 pad1 : 23; } Flags;
typedef struct Object { Pair position; u8 pad8[4]; Rect field0C; u8 pad1C[2]; u8 field1E, field1F; Flags field20; Entry *field24; u8 pad28[0x30]; struct Object *field58; u8 pad5C[0x2D]; u8 field89; u8 pad8A[0x10]; u16 field9A; u8 pad9C[0x48]; u16 fieldE4; } Object;
extern u8 D_8014344C;
typedef struct { void *room; s32 opaque[3]; } RoomState;
extern RoomState D_80143434;
extern void *D_801476B8;
extern s32 func_800A65B8(Object *, Object *);
extern s32 func_800E1CC4(Object *, s32);
extern s32 func_800A6FD0(Object *);
extern s32 func_800F069C(Object *);
extern s32 func_800B60D0(void);
extern s32 func_800A650C(Object *, Pair *);
extern Pair *func_800A2644(Pair *out, Pair *from, Pair *to, s32 range);
extern void *func_800A2FD0(void *output, void *input, u8 index);
extern void func_800A3448(s32 *arg0, s32 *arg1);
extern s32 func_800A4520(Object *, Object *);
extern s32 func_800E0F40(Object *);
extern Pair *func_800B33BC(Pair *out, Rect *desc, u8 kind, s32 arg3);
extern void func_800A59A4(Object *);
extern Pair func_800A694C(Object *obj, Rect *bounds, u8 mask, s32 arg4);
extern void func_800A58FC(Object *actor, Pair *position);
extern s32 func_800E20CC(Object *);
static inline void copy(Pair *dest, const Pair *source) {
    dest->x = source->x;
    dest->y = source->y;
}
static inline s32 nonzero(Pair *position) { return position->y | position->x; }
static inline void center(Pair *dest, Rect *rect) {
    s32 x = rect->minimum.x + rect->maximum.x;
    s32 y = rect->minimum.y + rect->maximum.y;
    dest->x = x / 2;
    dest->y = y / 2;
}
static inline void copy_flags(Flags *dest, const Flags *source) {
    *dest = *source;
}
Pair *func_80104040(Pair *result, Object *obj, Object *source) {
    Pair origin, best, temp;
    Rect bounds;
    Flags flags;
    Object *target;
    s32 valid = 0, nearby;
    copy(&origin, &obj->position);
    best.x = 0;
    best.y = 0;
    target = obj->field58;
    if (source != target && target && func_800A65B8(obj, target) <= obj->field89) {
        if (!(target->field1E & 0x7C) || !func_800E1CC4(target, 1)) valid = 1;
    }
    if (valid) {
        copy(result, &target->position);
        goto done;
    }
    nearby = 0;
    if (func_800A6FD0(source) && source == obj->field58 && func_800F069C(obj)
        && !func_800E1CC4(obj, 4) && D_8014344C >= 2 && func_800B60D0()) {
        nearby = !D_80143434.opaque[1];
    }
    if (nearby) {
        Rect *rect = D_80143434.room;
        center(&temp, rect);
        if (func_800A650C(obj, &temp) <= obj->field89) {
            copy(result, &temp);
            goto done;
        }
        {
            s32 x, y;
            if (rect->minimum.y > origin.y) y = rect->minimum.y - origin.y;
            else if (rect->maximum.y < origin.y) y = origin.y - rect->maximum.y;
            else y = 0;
            if (rect->minimum.x > origin.x) x = rect->minimum.x - origin.x;
            else if (rect->maximum.x < origin.x) x = origin.x - rect->maximum.x;
            else x = 0;
            if (y < x) y = x;
            if (y <= obj->field89) {
                func_800A2644(result, &origin, &temp, obj->field89);
                goto done;
            }
        }
    }
    func_800A2FD0(&bounds, &origin, obj->field89);
    func_800A3448((s32 *)&bounds, (s32 *)&obj->field0C);
    if (func_800A4520(obj, source)) {
        s32 allow = 0;
        if ((u8)func_800E0F40(obj) >= 2) allow = 1;
        else if (obj->field9A & 0x40) {
            Object *active = D_801476B8;
            s32 bit = active->field20.bit23;

            copy_flags(&flags, &active->field20);
            if (bit || ((active->fieldE4 >> 3) & 1)) allow = 1;
        }
        func_800B33BC(&temp, &bounds, 0x10, allow);
        best = temp;
    }
    if (!nonzero(&best)) {
        s32 failed;
        target = obj->field58;
        failed = func_800F069C(obj) ^ 1;
        if (failed) {
            s32 fallback;
            func_800A59A4(source);
            temp = func_800A694C(obj, &bounds, 0xFF, 1);
            best = temp;
            func_800A58FC(source, &source->position);
            fallback = !nonzero(&best) && func_800E20CC(obj);
            if (fallback) best = source->position;
        } else if (source == target) {
            s32 rejected = source->field24[2].call((u8 *)source + source->field24[2].adjust) ^ 1;
            if (rejected) {
                temp = func_800A694C(obj, &bounds, 0x7C, 0);
                best = temp;
            }
        }
    }
    copy(result, &best);
done: /* single exit: every path leaves through one return of result */
    return result;
}
