#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;

typedef struct { s32 x; s32 y; } Pos801249E0;
typedef struct { u8 pad0[0x40]; Pos801249E0 *target_40; u8 pad44[0xC]; } Path801249E0;
typedef struct { s32 a; s32 b; } Tmp801249E0;
typedef struct { u8 value; } Dir801249E0;
typedef struct ShirenDirection { s8 value; } ShirenDirection;
extern u8 D_80147620[];
extern u8 D_80156A61;
extern u8 D_80156A63;
extern u16 D_80156A8C;
void *func_801156DC(void *out, void *owner, void *position, void *dir, s32 range);
void *func_800C4360(void *self, void *owner, u16 value, s32 command, void *position, ShirenDirection direction, s32 limit, u16 flags, u8 mode);
void func_800C2D0C(Path801249E0 *path);
s32 func_800A7D20(Pos801249E0 *target);
void *func_800A7044(void *out_position, void *obj, void *direction, s32 range, s32 stop_on_terrain);
s32 func_80049CB4(s32 id, ...);
void func_80115E18(void *obj, void *position);
s32 func_800C5844(void *rng, u8 base, u8 top);
u16 func_80115944(void *owner, u16 value);
void func_800A7204(void *target, void *source, void *direction, s32 range, s32 damage, s32 message_kind, s32 stop_on_terrain, s32 notify);

static inline Pos801249E0 *getTarget801249E0(Path801249E0 *path) {
    return path->target_40;
}

/* Trap slot +0x44 also supplies a target unit and item; this override ignores both. */
s32 func_801249E0(void *owner, void *a, void *b, void *c, Dir801249E0 *facing, void *unused5, void *unused6) {
    Pos801249E0 pos;
    Path801249E0 path;
    Tmp801249E0 tmp;
    Pos801249E0 copy;
    ShirenDirection dir;
    Pos801249E0 *target;

    dir.value = (facing->value - 2) & 7;
    func_801156DC(&pos, owner, c, &dir, 100);
    dir.value = ((u8)dir.value + 4) & 7;
    func_800C4360(&path, a, 0xFA, 0xEE, &pos, dir, 0xFF, 4, 1);
    func_800C2D0C(&path);
    target = getTarget801249E0(&path);
    if (target != 0) {
        s32 speed = 3;

        if (func_800A7D20(target) == 0) {
            speed = D_80156A8C;
        }
        func_800A7044(&tmp, target, &dir, speed, 0);
        copy.x = target->x;
        copy.y = target->y;
        func_80049CB4(6);
        func_80115E18(owner, b);
        func_80049CB4(7);
        func_80049CB4(6);
        func_80049CB4(0xA1, target);
        func_80049CB4(0xEE, &copy, &tmp);
        func_80049CB4(7);
        func_800A7204(target, a, &dir, speed,
                      (u16)func_80115944(owner, (u8)func_800C5844(D_80147620, D_80156A61, D_80156A63)), 7, 0, 1);
    }
    return 1;
}
