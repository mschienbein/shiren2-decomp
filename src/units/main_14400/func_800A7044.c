#include "common.h"
#include "map_line_iterator.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 value; } Dir;
/* Partial unit view: position +0, mode byte +9, flags +0x1C; not allocated here. */
typedef struct { MapPos pos; u8 pad8; u8 field_9; u8 padA[0x12]; u16 flags; } Unit800A7044;
u32 func_800B1C6C(void *pos);
void *func_800A2594(void *out, void *from, Dir dir);
void func_800A2758(MapPos *pos, Dir dir);
s32 func_800A4754(Unit800A7044 *unit, MapPos *pos, Dir *dir);
s32 func_800A4404(Unit800A7044 *unit, MapPos *pos, void **hit);

static inline void Pos_copy(MapPos *dst, MapPos *src) {
    dst->x = src->x;
    dst->y = src->y;
}

MapPos *func_800A7044(MapPos *out, Unit800A7044 *unit, Dir *dir, s32 range, s32 flag) {
    MapPos pos;
    MapPos start;
    MapPos cur;
    MapLineIterator iter;
    void *hit;
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
    func_800C25D0(&iter, &cur, &dir->value, range);
    while (1) {
        if (iter.index >= iter.length) break;
        func_800C2758(&cur, &iter);
        hit = 0;
        stop = 0;
        if (!func_800A4754(unit, &start, dir) || !func_800A4404(unit, &cur, &hit)
            || (flag && (func_800B1C6C(&cur) & 0x2000))) {
            stop = 1;
        }
        if (stop) {
            if (hit == 0 || ((((Unit800A7044 *)hit)->flags & 1) ^ 1) == 0) {
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
