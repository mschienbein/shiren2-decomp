#include "common.h"

typedef unsigned char u8;

typedef struct { s32 x; s32 y; } Pos800E7104;
typedef struct { u8 value; } Dir;
typedef struct { u32 pad : 5; u32 wary : 1; u32 rest : 26; } Flags800E7104;
typedef struct {
    Pos800E7104 pos;
    Dir dir;
    u8 pad9[0x15];
    u8 bits;
    u8 pad1F;
    Flags800E7104 flags;
} Unit800E7104;

s32 func_800E6364(Unit800E7104 *unit, Pos800E7104 *from, Pos800E7104 *to, s32 mode);
void func_800A4EC0(void *p, void *a);
void *func_800A27A4(void *out_direction, void *from, void *to);
void *func_800A2594(void *out, void *from, Dir dir);
s32 func_800B56F0(void *object);
void func_800A665C(Unit800E7104 *unit, Dir *dir);
s32 func_800A5018(Unit800E7104 *unit, s32 kind);

static inline s32 flags_wary(Flags800E7104 *flags)
{
    return flags->wary;
}

s32 func_800E65C0(Unit800E7104 *unit, Pos800E7104 *target, s32 mode)
{
    Pos800E7104 from;
    Pos800E7104 to;
    Dir dir;
    Dir step;
    Flags800E7104 flags;
    Pos800E7104 *p;
    s32 found;
    s32 blocked;

    from.x = unit->pos.x;
    p = &from;
    p->y = unit->pos.y;
    found = func_800E6364(unit, &from, target, 0);
    if (found != -1) {
        dir.value = found & 7;
        func_800A4EC0(unit, &dir);
        return 1;
    }
    if (mode != 0) {
        return 0;
    }
    to.x = target->x;
    to.y = target->y;
    func_800A27A4(&step, &from, &to);
    blocked = 0;
    flags = unit->flags;
    if (flags_wary(&flags)) {
        func_800A2594(&to, &from, step);
        blocked = func_800B56F0(&to) != 0;
    }
    if (!blocked) {
        func_800A665C(unit, &step);
        if (func_800A5018(unit, 0) != 0) {
            return 1;
        }
    }
    return 0;
}
