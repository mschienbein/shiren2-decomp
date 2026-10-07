#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x; s32 y; } Pos800A7044;
typedef struct { u8 value; } Dir;
typedef struct { s32 field_0; s32 field_4; s32 field_8; s32 count; s32 index; s32 field_14; } Iter800A7044;
typedef struct { Pos800A7044 pos; u8 pad8; u8 field_9; u8 padA[0x12]; u16 flags; } Unit800A7044;
u32 func_800B1C6C(void *pos);
void *func_800A2594(void *out, void *from, Dir dir);
void func_800A2758(Pos800A7044 *pos, Dir dir);
void func_800C25D0(Iter800A7044 *iter, Pos800A7044 *pos, Dir *dir, s32 range);
void *func_800C2758(void *out, void *iterator);
s32 func_800A4754(Unit800A7044 *unit, Pos800A7044 *pos, Dir *dir);
s32 func_800A4404(Unit800A7044 *unit, Pos800A7044 *pos, Unit800A7044 **hit);

static inline void Pos_copy(Pos800A7044 *dst, Pos800A7044 *src) {
    dst->x = src->x;
    dst->y = src->y;
}

Pos800A7044 *func_800A7044(Pos800A7044 *out, Unit800A7044 *unit, Dir *dir, s32 range, s32 flag) {
    Pos800A7044 pos;
    Pos800A7044 start;
    Pos800A7044 cur;
    Iter800A7044 iter;
    Unit800A7044 *hit;
    s32 blocked;
    s32 stop;

    Pos_copy(&pos, &unit->pos);
    blocked = 0;
    if ((unit->flags & 2) || ((func_800B1C6C(&pos) & 0x80) && (unit->field_9 & 0xF) == 1)) blocked = 1;
    if (blocked) {
        Pos_copy(out, &pos);
    } else {
    Pos_copy(&start, &pos);
    func_800A2594(&cur, &pos, *dir);
    func_800C25D0(&iter, &cur, dir, range);
    while (1) {
        if (iter.index >= iter.count) break;
        func_800C2758(&cur, &iter);
        hit = 0;
        stop = 0;
        if (!func_800A4754(unit, &start, dir) || !func_800A4404(unit, &cur, &hit)
            || (flag && (func_800B1C6C(&cur) & 0x2000))) {
            stop = 1;
        }
        if (stop) {
            if (hit == 0 || ((hit->flags & 1) ^ 1) == 0) {
                Dir back;
                back.value = (dir->value + 4) & 7;
                func_800A2758(&cur, back);
            }
            break;
        }
    }
    Pos_copy(out, &cur);
    }
    return out;
}
