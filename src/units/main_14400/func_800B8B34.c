#include "common.h"
typedef unsigned char u8;
typedef struct { s32 x; s32 y; } Pos800B8B34;
typedef struct { u8 value; } Dir800B8B34;
typedef struct { u8 state; u8 walls; u8 x; u8 y; } Cell800B8B34;
typedef struct { Pos800B8B34 min, max; } Rect800B8B34;
typedef struct {
    u8 pad0[0x10];
    u8 rows;
    u8 cols;
    u8 wanted;
    Cell800B8B34 cells[11][8];
    u8 pad173;
    s32 available[11][8];
    signed char *field_2D4;
    signed char *field_2D8;
    Rect800B8B34 rooms_2DC[16];
    s32 room_count_3DC;
    u8 field_3E0;
    u8 top;
    u8 left;
    u8 bottom;
    u8 right;
    u8 pad3E5[7];
    Pos800B8B34 start;
    s32 field_3F4;
    s32 field_3F8;
} Map800B8B34;
extern u8 D_80147620[];
s32 func_800C5844(void *rng, u8 min, u8 max);
s32 func_800C587C(void *rng, u8 percent);
void func_800B90E8(Map800B8B34 *map, u8 row, u8 col);
void func_800B8F94(Map800B8B34 *map, u8 row, u8 col);
s32 func_800A251C(Pos800B8B34 *pos, Pos800B8B34 *start);
void *func_800A2594(Pos800B8B34 *out, Pos800B8B34 *pos, Dir800B8B34 dir);
s32 func_800B89D8(Map800B8B34 *map, u8 row, u8 col, u8 kind);

static inline Dir800B8B34 Dir_make(u8 value) {
    Dir800B8B34 dir;
    dir.value = value;
    return dir;
}

static inline void Dir_set(Dir800B8B34 *dir, u8 value) {
    dir->value = value;
}

#define ROWS(map) ((map)->rows)
#define COLS(map) ((map)->cols)

void func_800B8B34(Map800B8B34 *map) {
    u8 row;
    u8 col;
    s32 done;
    Pos800B8B34 pos;
    Pos800B8B34 next;
    Dir800B8B34 dir1;
    Dir800B8B34 dir2;
    Dir800B8B34 dir3;
    Dir800B8B34 dir4;

    do {
        row = func_800C5844(&D_80147620, 1, ROWS(map));
        col = func_800C5844(&D_80147620, 1, COLS(map));
    } while ((row == map->start.y && col == map->start.x) || map->cells[row][col].state != 0x80);
    map->cells[row][col].state = 0x10;
    func_800B90E8(map, row, col);

    do {
        done = 1;
        for (row = 1; row <= ROWS(map); row++) {
            for (col = 1; col <= COLS(map); col++) {
                u8 flag = map->cells[row][col].state;
                if (flag == 0x80) {
                    done = 0;
                } else if (flag == 0x10) {
                    func_800B8F94(map, row, col);
                }
            }
        }
    } while (!done);

    if (map->field_3E0 != 1 && map->field_3F8 == 0 && func_800C587C(&D_80147620, 0x50) == 0) return;
    map->field_3F4 = 1;

    pos.x = map->left;
    pos.y = map->top;
    while (1) {
        s32 here;
        s32 there;
        if (pos.y > map->bottom - 1) break;
        here = func_800A251C(&pos, &map->start);
        Dir_set(&dir1, 0);
        func_800A2594(&next, &pos, dir1);
        there = func_800A251C(&next, &map->start);
        if (here || there) {
            if (!func_800B89D8(map, pos.y, pos.x + 1, 1)) map->field_3F4 = 0;
        } else {
            if (!func_800B89D8(map, pos.y, pos.x, 1)) map->field_3F4 = 0;
        }
        pos.y++;
    }
    pos.x = map->right;
    pos.y = map->top;
    while (1) {
        s32 here;
        s32 there;
        if (pos.y > map->bottom - 1) break;
        here = func_800A251C(&pos, &map->start);
        Dir_set(&dir2, 0);
        func_800A2594(&next, &pos, dir2);
        there = func_800A251C(&next, &map->start);
        if (here || there) {
            if (!func_800B89D8(map, pos.y, pos.x - 1, 1)) map->field_3F4 = 0;
        } else {
            if (!func_800B89D8(map, pos.y, pos.x, 1)) map->field_3F4 = 0;
        }
        pos.y++;
    }
    pos.y = map->top;
    pos.x = map->left;
    while (1) {
        s32 here;
        s32 there;
        Pos800B8B34 *destination; /* FAKEMATCH: This redundant alias orders the
                                    preheader's &next/addiu before direction-6/li.
                                    Direct &next misses four words overall;
                                    C++ returned-out-pointer, placement construction
                                    (also consuming its result), and by-value Point
                                    return spellings were tried and did not match. */
        if (pos.x > map->right - 1) break;
        here = func_800A251C(&pos, &map->start);
        destination = &next;
        dir3 = Dir_make(6);
        func_800A2594(destination, &pos, dir3);
        there = func_800A251C(&next, &map->start);
        if (here || there) {
            if (!func_800B89D8(map, pos.y + 1, pos.x, 8)) map->field_3F4 = 0;
        } else {
            if (!func_800B89D8(map, pos.y, pos.x, 8)) map->field_3F4 = 0;
        }
        pos.x++;
    }
    pos.y = map->bottom;
    pos.x = map->left;
    while (1) {
        s32 here;
        s32 there;
        Pos800B8B34 *destination; /* FAKEMATCH: This redundant alias orders the
                                    preheader's &next/addiu before direction-6/li.
                                    Direct &next misses four words overall;
                                    C++ returned-out-pointer, placement construction
                                    (also consuming its result), and by-value Point
                                    return spellings were tried and did not match. */
        if (pos.x > map->right - 1) break;
        here = func_800A251C(&pos, &map->start);
        destination = &next;
        dir4 = Dir_make(6);
        func_800A2594(destination, &pos, dir4);
        there = func_800A251C(&next, &map->start);
        if (here || there) {
            if (!func_800B89D8(map, pos.y - 1, pos.x, 8)) map->field_3F4 = 0;
        } else {
            if (!func_800B89D8(map, pos.y, pos.x, 8)) map->field_3F4 = 0;
        }
        pos.x++;
    }
}
