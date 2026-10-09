#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 value;
} Dir80124230;

typedef struct {
    s32 x;
    s32 y;
} Pos80124230;

typedef struct {
    char pad0[8];
    short delta_8;
    short index_A;
    void (*destroy_C)(void *self, s32 flags);
} VTable80124230;

typedef struct {
    u8 pad0[0xA];
    u8 floor_A;
    u8 padB[0x24 - 0xB];
    VTable80124230 *vtable_24;
} Obj80124230;

extern char D_80147620[];
Obj80124230 *func_800AA684(void);
s32 func_800E0F40(Obj80124230 *obj);
void *func_800A8694(u8 floor, u8 room, void *mem);
u8 func_800C57CC(void *rng, s32 limit);
void *func_800A2594(Pos80124230 *out, void *origin, Dir80124230 dir);
s32 func_800A4314(Obj80124230 *obj, Pos80124230 *pos);
void func_800A2F80(u8 *dir, s32 step);
s32 func_80049CB4(s32 id, ...);
void func_800A58FC(void *actor, Pos80124230 *pos);
void func_800498E4(s32 id, ...);

/* D_8015FE28+0x44 (0x8015FE6C), installed by func_801241D0.
 * func_80115EB0 supplies seven pointers at 0x80116028-0x80116050 and
 * consumes the s32 result at 0x80116058/0x801160C0. Only the effect
 * position in a3 is used; self, actor, source_position, direction,
 * target and item are caller-supplied but unused here. */
s32 func_80124230(void *self, void *actor, Pos80124230 *source_position,
                 Pos80124230 *origin, Dir80124230 *direction,
                 void *target, void *item) {
    s32 i;
    s32 tries;
    s32 placed;
    u8 room;
    u8 floor;
    Obj80124230 *obj;
    Dir80124230 dir;
    Pos80124230 pos;

    floor = 0;
    room = 1;
    placed = 0;
    tries = 0;
    obj = func_800AA684();
    if (obj != 0) {
        floor = obj->floor_A;
        room = func_800E0F40(obj);
        tries = 4;
        obj->vtable_24->destroy_C((char *)obj + obj->vtable_24->delta_8, 3);
    }
    while (1) {
        if (--tries == -1) break;
        obj = func_800A8694(floor, room, 0);
        if (obj == 0) {
            continue;
        }
        dir.value = func_800C57CC(D_80147620, 7) & 7;
        i = 8;
        while (1) {
            if (--i == -1) break;
            func_800A2594(&pos, origin, dir);
            if (func_800A4314(obj, &pos)) {
                func_80049CB4(6);
                func_80049CB4(0x82, obj, &pos);
                func_800A58FC(obj, &pos);
                func_80049CB4(7);
                placed++;
                break;
            }
            func_800A2F80(&dir.value, 1);
        }
        if (i < 0 && obj != 0) {
            obj->vtable_24->destroy_C((char *)obj + obj->vtable_24->delta_8, 3);
        }
    }
    if (placed == 0) {
        func_800498E4(0x223);
    }
    return 1;
}
