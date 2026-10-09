#include "common.h"

typedef unsigned char u8;

typedef struct Pos80126D2C {
    s32 x;
    s32 y;
} Pos80126D2C;

typedef struct Dir80126D2C {
    u8 value;
} Dir80126D2C;

typedef struct Methods80126D2C {
    u8 pad_00[8];
    short delta_08;
    short index_0A;
    void (*destroy_0C)(void *self, s32 flags);
} Methods80126D2C;

/* Partial view of the spawned monster: vtable at 0x24, flags at 0x72. */
typedef struct Monster80126D2C {
    u8 pad_00[0x24];
    Methods80126D2C *vtable_24;
    u8 pad_28[0x72 - 0x28];
    u8 flags_72;
} Monster80126D2C;

/* Partial view of this item: vtable at 0x08, monster kind at 0x0F. */
typedef struct Self80126D2C {
    u8 pad_00[8];
    Methods80126D2C *vtable_08;
    u8 pad_0C[3];
    u8 kind_0F;
} Self80126D2C;

typedef struct Msg80126D2C {
    s32 type_00;
    void *target_04;
} Msg80126D2C;

s32 func_80112B38(void *self, void *msg);
void *func_800F8720(u8 value);
s32 func_800A3934(void *obj);
char *func_800A3B20(void *u);
void func_800498E4(s32 id, ...);
s32 func_80049CB4(s32 id, ...);
void *func_800A6CC0(Pos80126D2C *out_position, void *obj);
s32 func_800A4314(void *o, Pos80126D2C *p);
char *func_800AE674(void *obj);
void func_800A58FC(void *actor, Pos80126D2C *position);
void *func_800A65E4(Dir80126D2C *p, void *q, void *target);
void func_800A665C(void *obj, u8 *value);
void func_800D3650(void *arg);

/* Message handler: on message 6, release the monster of kind_0F next to the target. */
s32 func_80126D2C(Self80126D2C *self, Msg80126D2C *msg)
{
    Pos80126D2C pos;
    Dir80126D2C dir;
    void *target;
    Monster80126D2C *monster;
    s32 placed;

    if (msg->type_00 == 6) {
        target = msg->target_04;
        monster = func_800F8720(self->kind_0F);
        if (func_800A3934(monster)) {
            func_800498E4(0x118, func_800A3B20(target));
            return 0;
        }
        func_800A6CC0(&pos, target);
        placed = func_800A4314(monster, &pos) == 1;
        if (!placed) {
            func_800498E4(0x119);
            if (monster != 0) {
                monster->vtable_24->destroy_0C((u8 *)monster + monster->vtable_24->delta_08, 3);
            }
            return 0;
        }
        func_800498E4(0x117, func_800A3B20(target), func_800AE674(self));
        func_80049CB4(0x28, target);
        func_800A58FC(monster, &pos);
        func_800A65E4(&dir, monster, target);
        func_800A665C(monster, &dir.value);
        monster->flags_72 |= 4;
        func_800D3650(self);
        if (self != 0) {
            self->vtable_08->destroy_0C((u8 *)self + self->vtable_08->delta_08, 3);
        }
        return 1;
    }
    return func_80112B38(self, msg);
}
