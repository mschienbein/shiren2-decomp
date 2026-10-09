#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct {
    s32 x;
    s32 y;
} Pos800E0534;

typedef struct {
    Pos800E0534 pos;
    u8 pad08[0x1E - 0x8];
    u8 flags_1E;
    u8 pad1F[0x28 - 0x1F];
    s16 hp;
} Unit800E0534;

extern u16 func_800E08B0(Unit800E0534 *obj);
extern u16 func_800E08F0(Unit800E0534 *obj);
extern char *func_800A3B20(Unit800E0534 *u);
extern s32 func_80049CB4(s32 id, ...);
extern void func_800497F0(s32, ...);

static inline void copy_pos(Pos800E0534 *to, Pos800E0534 *from) {
    to->x = from->x;
    to->y = from->y;
}

/* Add amount to the unit's current value (clamped to [0, max]); report the change. */
s32 func_800E0534(Unit800E0534 *obj, s32 amount) {
    u16 old = func_800E08B0(obj);
    s16 value = old + amount;
    s16 hp = value;
    u16 max = func_800E08F0(obj);
    char *name;
    s32 effect;

    if (value > max) {
        hp = max;
    } else if (value < 0) {
        hp = 0;
    }
    obj->hp = hp;
    name = func_800A3B20(obj);
    if (hp != old) {
        Pos800E0534 pos;

        copy_pos(&pos, &obj->pos);
        effect = func_80049CB4(0xDA, &pos);
        if (amount < 0) {
            func_80049CB4(0x24, obj);
            if (obj->flags_1E & 0xC) {
                func_800497F0(4, effect, name, -amount);
            } else {
                func_800497F0(5, effect, name, -amount);
            }
        } else if (amount > 0) {
            func_80049CB4(0x44, obj);
            if (obj->flags_1E & 0xC) {
                func_800497F0(2, effect, name, amount);
            } else {
                func_800497F0(3, effect, name, amount);
            }
        }
    }
    return hp - old;
}
