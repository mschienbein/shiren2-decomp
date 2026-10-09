#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct { s32 x, y; } Pos;

typedef struct Message {
    void *field_0;
    s32 field_4;
    s32 field_8;
    s16 field_C;
    u16 field_E;
    u8 field_10;
} Message;

typedef struct {
    u8 pad0[0x68];
    s16 delta_68;
    s16 index_6A;
    /* D_801496E0+0x6C binds the u32 result of func_800E0E88. */
    u32 (*method_6C)(void *self);
} UnitVTable800F4498;

typedef struct Unit800F4498 {
    Pos pos;
    u8 dir_8;
    u8 pad9[0x24 - 9];
    UnitVTable800F4498 *vtable;
    u8 pad28[0x58 - 0x28];
    struct Unit800F4498 *target_58;
    u8 pad5C[0x7C - 0x5C];
    u16 field_7C;
} Unit800F4498;

extern s32 func_800E20CC(void *arg0);
extern void *func_800A6CC0(void *out_position, void *obj);
extern void *func_800B4928(Pos *pos);
extern u8 func_800A6420(Unit800F4498 *obj, Unit800F4498 *target);
extern void *func_800A65E4(u8 *out, Unit800F4498 *obj, void *target);
extern s32 func_800E1CC4(Unit800F4498 *obj, s32 kind);
extern void func_800A665C(Unit800F4498 *obj, u8 *value);
extern s32 func_800E33D0(Unit800F4498 *arg, Message message, u8 rays, u8 range);

s32 func_800F4498(Unit800F4498 *unit) {
    Pos pos;
    Message message;
    Message copy;
    u8 facing;
    u8 dir;

    if (func_800E20CC(unit)) {
        func_800A6CC0(&pos, unit);
        func_800B4928(&pos);
        dir = unit->dir_8;
    } else {
        Unit800F4498 *target = unit->target_58;

        if (func_800A6420(unit, target) != 0) {
            return 0;
        }
        func_800A65E4(&facing, unit, target);
        dir = facing;
    }
    if (func_800E1CC4(unit, 4)) {
        dir = (dir + 4) & 7;
    }
    func_800A665C(unit, &dir);
    message.field_C = unit->field_7C + unit->vtable->method_6C((u8 *)unit + unit->vtable->delta_68);
    message.field_E = 0;
    message.field_8 = 0;
    message.field_10 = 0xA;
    copy = message;
    func_800E33D0(unit, copy, 1, 1);
    return 1;
}
