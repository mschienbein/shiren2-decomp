#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct {
    s32 x;
    s32 y;
} Pos_800F3E68;

typedef struct {
    Pos_800F3E68 pos_0;
    u8 pad8[0x2A];
    u8 hp_32;
    u8 pad33[0x42];
    u8 field_75;
} Obj_800F3E68;

static inline void pos_copy(Pos_800F3E68 *dst, Pos_800F3E68 *src) {
    dst->x = src->x;
    dst->y = src->y;
}

extern u16 func_800E08B0(Obj_800F3E68 *obj);
extern s32 func_800E0F40(Obj_800F3E68 *obj);
extern char *func_800A3B20(Obj_800F3E68 *obj);
extern s32 func_80049CB4(s32 command, ...);
extern void func_800A7C1C(Obj_800F3E68 *obj);
extern void func_800498E4(s32 id, ...);
extern void func_800E075C(Obj_800F3E68 *obj);

s16 func_800F3E68(Obj_800F3E68 *obj, s16 amount, u8 max) {
    Pos_800F3E68 pos;
    s32 old;
    s16 hp;
    s16 diff;
    char *name;
    s32 message;

    if (amount == 0 || func_800E08B0(obj) == 0) {
        return 0;
    }
    old = (u8)func_800E0F40(obj);
    hp = obj->hp_32 + amount;
    if (max < hp) {
        hp = max;
    } else if (hp <= 0) {
        hp = 1;
    }
    diff = hp - old;
    if (diff != 0) {
        name = func_800A3B20(obj);
        func_80049CB4(0x132);
        obj->hp_32 = hp;
        obj->field_75 = func_800E0F40(obj);
        func_80049CB4(6);
        if (diff > 0) {
            func_80049CB4(0x80, obj);
            message = 0x27;
        } else {
            pos_copy(&pos, &obj->pos_0);
            func_80049CB4(0x109, &pos);
            message = 0x28;
        }
        func_80049CB4(7);
        func_800A7C1C(obj);
        func_80049CB4(6);
        func_80049CB4(0x20, obj, diff);
        func_80049CB4(7);
        func_800498E4(message, name, func_800A3B20(obj));
        func_800E075C(obj);
    }
    return diff;
}
