#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;

typedef struct { s32 x; s32 y; } Pos800E7104;
typedef struct { u8 value; } Dir;
typedef struct { u32 pad : 5; u32 wary : 1; u32 rest : 26; } Flags800E7104;
typedef struct {
    s32 x;
    s32 y;
    Dir dir;
    u8 pad9[0x15];
    u8 bits;
    u8 pad1F;
    Flags800E7104 flags;
    u8 pad24[0x30];
    u8 status;
    u8 pad55[0x3];
    Pos800E7104 *dest;
    u8 pad5C[0x8];
    Pos800E7104 last;
    u8 pad6C[0x7];
    u8 state;
    u8 pad74[0x2];
    u8 idle;
} Unit800E7104;

s32 func_800F069C(void *obj);
s32 func_800E66EC(Unit800E7104 *unit);
Pos800E7104 *func_800C5F60(void);
u8 func_800A6420(Unit800E7104 *unit, Pos800E7104 *dest);
void *func_800A65E4(Dir *out, Unit800E7104 *unit, Pos800E7104 *dest);
void func_800A665C(Unit800E7104 *unit, Dir *dir);
s32 func_800E65C0(Unit800E7104 *unit, Pos800E7104 *target, s32 mode);
void *func_800A2594(void *out, void *from, Dir dir);
s32 func_800B56F0(Pos800E7104 *pos);
s32 func_800A23E8(Pos800E7104 *a, Pos800E7104 *b);
s32 func_800E62BC(Unit800E7104 *unit, Pos800E7104 *target, s32 range);
s32 func_800E6364(Unit800E7104 *unit, Pos800E7104 *from, Pos800E7104 *to, s32 mode);
s32 func_800A251C(Pos800E7104 *pos, Unit800E7104 *unit);
s32 func_800A41EC(Unit800E7104 *unit, Pos800E7104 *out);
u32 func_800B1C6C(Pos800E7104 *pos);

static inline s32 is_wary(Flags800E7104 *flags) {
    return flags->wary;
}

s32 func_800E7104(Unit800E7104 *unit) {
    Pos800E7104 here;
    Pos800E7104 target;
    Pos800E7104 tmp;
    Dir face1;
    Flags800E7104 flags;
    Dir face2;
    Pos800E7104 *dest = unit->dest;
    Pos800E7104 *hp;
    Pos800E7104 *last;
    s32 result;
    s32 dir;

    if (dest == 0) {
        s32 ok = 0;
        u32 can_wander = (unit->bits >> 4) & 1;
        if (can_wander != 0) {
            ok = func_800F069C(unit) != 0;
        }
        if (!ok) {
            unit->state = 3;
            return func_800E66EC(unit);
        }
        dest = func_800C5F60();
    }
    result = 0;
    here.x = unit->x;
    hp = &here;
    hp->y = unit->y;
    target.x = dest->x;
    target.y = dest->y;
    switch (func_800A6420(unit, dest)) {
    case 0:
        func_800A65E4(&face1, unit, dest);
        func_800A665C(unit, &face1);
        unit->idle = 0;
        unit->status |= 4;
        break;
    case 1:
    case 2: {
        s32 stop;
        s32 face;
        unit->idle = 0;
        result = func_800E65C0(unit, &target, 1);
        if (result != 0) {
            break;
        }
        stop = 0;
        flags = unit->flags;
        if (is_wary(&flags)) {
            func_800A2594(&tmp, hp, unit->dir);
            stop = func_800B56F0(&tmp) != 0;
        }
        if (stop) {
            return 0;
        }
        result = func_800E66EC(unit);
        face = 0;
        if (result == 0 && !(unit->status & 4)) {
            face = unit->idle >= 11;
        }
        if (face) {
            func_800A65E4(&face2, unit, dest);
            func_800A665C(unit, &face2);
        }
        break;
    }
    default:
        tmp.x = target.x;
        tmp.y = target.y;
        if (func_800A23E8(&here, &tmp) < 4 && func_800E62BC(unit, &target, 5)) {
            dir = func_800E6364(unit, &here, &target, 1);
            if (dir != -1) {
                /* relative turn; 3..5 means the target is behind */
                dir = (dir - unit->dir.value) & 7;
                if ((u32)(dir - 3) >= 3) {
                    unit->last.x = 0;
                    unit->last.y = 0;
                    result = func_800E65C0(unit, &target, 0);
                    if (result != 0) {
                        break;
                    }
                }
            }
        }
        result = func_800E66EC(unit);
        if (func_800A251C(&here, unit)) {
            unit->idle += 2;
        }
        break;
    }
    unit->state = func_800A6420(unit, dest);
    if (unit->state != 3) {
        if (func_800A41EC(unit, &target)) {
            unit->last = target;
        }
        return result;
    }
    {
        s32 clear = 0;
        tmp.x = unit->last.x;
        last = &unit->last;
        tmp.y = last->y;
        if ((tmp.y | tmp.x) != 0) {
            if (!(func_800B1C6C(&tmp) & 0x800)) {
                clear = 1;
            }
        }
        if (clear) {
            unit->last.x = 0;
            last->y = 0;
        }
    }
    return result;
}
