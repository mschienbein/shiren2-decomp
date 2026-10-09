#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

/* Entity method table at +0x24: slot +0x68/+0x6C u32 (void *self) (callers narrow to 16 bits). */
typedef struct {
    u8 pad00[0x68];
    s16 delta_68;
    s16 index_6A;
    u32 (*value_6C)(void *self);
} EntityVTable;

typedef struct {
    s32 x;
    s32 y;
} Pos;

typedef struct {
    Pos pos;
    u8 pad8[0x1E - 0x8];
    u8 state;
    u8 pad1F[0x24 - 0x1F];
    EntityVTable *vtable;
    u8 pad28[0x2C - 0x28];
    u16 hp;
    u16 maxHp;
} Obj;

extern s32 func_80049CB4(s32 id, ...);
extern char *func_800A3B20(void *actor);
extern void func_800497F0(s32 id, ...);

static inline void copy_pos(Pos *out, Pos *in) {
    out->x = in->x;
    out->y = in->y;
}

static inline u16 value(Obj *obj) {
    return obj->vtable->value_6C((char *)obj + obj->vtable->delta_68);
}

/* Add delta to HP (clamped to 1..maxHp) and report the resulting change. */
s32 func_800E0AB4(Obj *obj, s32 delta) {
    s32 change = value(obj); /* value before the update */
    s32 hp = delta + obj->hp;
    s32 where;
    char *name;
    Pos pos;

    if (hp <= 0) {
        hp = 1;
    } else if (hp > obj->maxHp) {
        hp = obj->maxHp;
    }
    obj->hp = hp;
    change = value(obj) - change;
    copy_pos(&pos, &obj->pos);
    where = func_80049CB4(0xDA, &pos);
    name = func_800A3B20(obj);
    if (change < 0) {
        if (obj->state & 0xC) {
            func_800497F0(0x1B, where, name, -change);
        } else {
            func_800497F0(0x1C, where, name, -change);
        }
    } else if (change > 0) {
        func_800497F0((obj->state & 0xC) ? 0x1D : 0x1E, where, name, change);
    }
    return change;
}

