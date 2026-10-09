#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { s32 x; s32 y; } Pos800F13A0;
typedef struct { u8 dir; } Dir800F13A0;
typedef struct { s8 d; } Dir;
typedef struct Unit800F13A0 {
    Pos800F13A0 pos;
    Dir800F13A0 dir;
    u8 pad9[0x4F];
    struct Unit800F13A0 *target;
} Unit800F13A0;


void *func_800B5A18(Pos800F13A0 *out, Pos800F13A0 *pos, Dir dir, s32 range, u16 flags);
s32 func_800E20CC(Unit800F13A0 *unit);
Pos800F13A0 *func_800F1B28(Pos800F13A0 *out, Unit800F13A0 *unit, s32 range, s32 arg3, s32 arg4);
u8 func_800A6420(Unit800F13A0 *unit, Unit800F13A0 *target);
s32 func_800A23E8(Pos800F13A0 *from, Pos800F13A0 *to);
Pos800F13A0 *func_800A2644(Pos800F13A0 *out, Pos800F13A0 *from, Pos800F13A0 *to, s32 range);
Pos800F13A0 *func_800A25D8(Pos800F13A0 *out, Pos800F13A0 *from, Dir800F13A0 dir, s32 range);
s32 func_800E1CC4(Unit800F13A0 *unit, s32 flag);
Pos800F13A0 *func_800A256C(Pos800F13A0 *out, Pos800F13A0 *from, Pos800F13A0 *to);
Pos800F13A0 *func_800A2544(Pos800F13A0 *out, Pos800F13A0 *from, Pos800F13A0 *to);
Pos800F13A0 *func_800F13A0(Pos800F13A0 *ret, Unit800F13A0 *unit, s32 range) {
    Pos800F13A0 from;
    Pos800F13A0 dest;
    Pos800F13A0 a;
    Pos800F13A0 b;
    Pos800F13A0 *fp;
    Unit800F13A0 *target;
    s32 special;
    fp = &from;
    fp->x = unit->pos.x;
    fp->y = unit->pos.y;
    special = D_80142F18.mode == 0x4F;
    if (special) {
        Dir dir;
        dest.x = fp->x;
        dest.y = fp->y;
        dir.d = unit->dir.dir;
        func_800B5A18(ret, &dest, dir, range, (u16)0xC000);
    } else {
        if (func_800E20CC(unit)) {
            func_800F1B28(&a, unit, range, 1, 1);
            dest = a;
        } else {
            target = unit->target;
            if (func_800A6420(unit, target) != 3) {
                a.x = target->pos.x;
                a.y = target->pos.y;
                b.x = a.x;
                b.y = a.y;
                if (range < func_800A23E8(&from, &b)) {
                    func_800A2644(&b, &from, &a, range);
                    dest = b;
                } else {
                    dest = a;
                }
            } else {
                func_800A25D8(&a, &from, unit->dir, range);
                dest = a;
            }
        }
        if (func_800E1CC4(unit, 4)) {
            func_800A256C(&b, &from, &dest);
            func_800A2544(&a, &from, &b);
            dest = a;
        }
        ret->x = dest.x;
        ret->y = dest.y;
    }
    return ret;
}
