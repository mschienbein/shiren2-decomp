#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct {
    u8 pad0[0x98];
    s16 delta_98;
    s16 index_9A;
    void *(*func_9C)(void *self);
} Vtable800EA08C;

typedef struct {
    u8 pad0[0x24];
    Vtable800EA08C *vtable;
} Obj800EA08C;

extern u16 D_80156912;
extern u16 D_80156914;
extern u16 D_8015691C;
extern u16 D_80156988;
extern u16 D_8015698A;
extern s32 func_800E1CC4(void *obj, s32 kind);
extern void *func_800E8A68(void *obj, u8 slot);
extern s32 func_8010BEC4(void *obj, u8 id);
extern s32 func_8010CF00(void *object, s32 kind, s32 value);
extern s32 func_800CF1C8(void *list, u8 kind);

static inline s32 func_800EA08C_boost(void *item, s32 value) {
    if ((u8)func_8010BEC4(item, 0x6A)) {
        return 100;
    }
    return value;
}

s32 func_800EA08C(Obj800EA08C *obj, s32 kind) {
    s32 result;
    void *found;
    void *item;
    void *list;
    s32 count;
    u8 rate;

    switch (kind) {
    case 0:
        result = 0;
        break;
    case 1:
        result = 100 - D_8015691C;
        break;
    case 2:
        result = 100 - D_80156912;
        break;
    case 3:
        result = 100 - D_80156914;
        break;
    case 4:
        if (func_800E1CC4(obj, 5) != 0) {
            return D_8015698A;
        }
        return D_80156988;
    default:
        return 0;
    }

    /* The slot lookup result (a0) and the item handed to the case helpers (v1) are two
       locals: the original copies one into the other at 0x800EA154 (daddu v1,a0,zero),
       ahead of the null test. */
    found = func_800E8A68(obj, 4);
    item = found;
    if (found != 0) {
        switch (kind) {
        case 0:
            result = func_8010CF00(item, 0, result);
            break;
        case 2:
        case 3:
            return func_800EA08C_boost(item, result);
        case 1:
            result = func_8010CF00(item, 1, result);
            break;
        }
    }

    if (kind < 2) {
        if (kind >= 0) {
            list = obj->vtable->func_9C((u8 *)obj + obj->vtable->delta_98);
            if (list != 0) {
                rate = 100 - result;
                count = func_800CF1C8(list, 0x8C) * 2;
                if (count > 0) {
                    rate = rate / count;
                }
                result = 100 - rate;
            }
        }
    }
    return result;
}
