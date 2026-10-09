#include "common.h"

/*
 * Fixed grid floor preset (C++ TU). Board::setup is the shared inline that
 * clamps its (modified) size parameters, selects the edge tables and runs
 * func_800B8628; the clamped parameters are what keep the 2 / 7 / 5 values in
 * registers (the sllv by a1 and the andi of the stored sizes).
 */

typedef unsigned char u8;
typedef signed char s8;

typedef struct {
    s32 y;
    s32 x;
} Point_800C1B50;

typedef struct {
    Point_800C1B50 min;
    Point_800C1B50 max;
} Rect_800C1B50;

typedef struct {
    u8 kind;
    u8 pad1[3];
} Cell_800C1B50;

typedef struct {
    u8 pad0[0x10];
    u8 width_10;
    u8 height_11;
    u8 field_12;
    Cell_800C1B50 cells_13[11][8];
    u8 pad173;
    s32 available_174[11][8];
    s8 *field_2D4;
    s8 *field_2D8;
    Rect_800C1B50 rooms_2DC[16];
    s32 room_count_3DC;
    void setup(u8 rows, u8 cols, u8 extra, u8 mode);
} Map_800C1B50;

extern "C" {
extern s8 *D_80153BDC[];
extern s8 *D_80153BF8[];
extern u8 D_80147620[];
extern u8 D_8014344C;
extern u8 D_80143392;
void func_800B8628(Map_800C1B50 *map, u8 mode);
s32 func_800C587C(void *rng, unsigned char chance);
void func_800BA188(void *map, u8 x, u8 y, s32 dir_a, s32 dir_b);
void func_800B90E8(Map_800C1B50 *map, u8 col, u8 row);
void func_800B95DC(Map_800C1B50 *map);
void func_800B17A4(void);
}

/* Clamps the grid size to 3..8 x 3..5, picks the edge tables, then lays out the cells. */
inline void Map_800C1B50::setup(u8 rows, u8 cols, u8 extra, u8 mode)
{
    if (rows < 3) {
        rows = 3;
    }
    if (cols < 3) {
        cols = 3;
    }
    if (rows >= 9) {
        rows = 8;
    }
    if (cols >= 6) {
        cols = 5;
    }
    width_10 = rows;
    height_11 = cols;
    s8 *row_table = D_80153BDC[width_10 - 3];
    s8 *col_table = D_80153BF8[height_11 - 3];
    field_2D4 = row_table;
    field_2D8 = col_table;
    field_12 = extra;
    func_800B8628(this, mode);
}

static inline s32 in_span(s32 value, s32 limit) {
    return value < limit;
}

static inline void point_init(Point_800C1B50 *point, s32 y, s32 x) {
    point->y = y;
    point->x = x;
}

static inline void rect_init(Rect_800C1B50 *rect, Point_800C1B50 *min, Point_800C1B50 *max) {
    rect->min = *min;
    rect->max = *max;
}

static inline void rect_assign(Rect_800C1B50 *dst, Rect_800C1B50 *src) {
    dst->min = src->min;
    dst->max = src->max;
}

static inline void add_room(Rect_800C1B50 *room, s32 y0, s32 x0, s32 y1, s32 x1) {
    Rect_800C1B50 rect;
    Point_800C1B50 min;
    Point_800C1B50 max;

    point_init(&min, y0, x0);
    point_init(&max, y1, x1);
    rect_init(&rect, &min, &max);
    rect_assign(room, &rect);
}

extern "C" void func_800C1B50(Map_800C1B50 *map) {
    s32 x;
    s32 y;
    u8 count;

    map->setup(6, 4, 0xB, 2);
    count = 0;
    for (y = 2; y < 4; y++) {
        for (x = 2; x < 6; x++) {
            map->cells_13[x][y].kind = 0x20;
        }
    }
    add_room(&map->rooms_2DC[count++], 2, 2, 3, 5);

    y = 1;
    for (x = 2; in_span(x, 6); x++) {
        if ((func_800C587C(D_80147620, 0x4B) ^ 1) != 0) {
            continue;
        }
        map->cells_13[x][y].kind = count | 0x20;
        add_room(&map->rooms_2DC[count++], y, x, y, x);
        func_800BA188(map, x, y, 8, 8);
    }
    y = 4;
    for (x = 2; in_span(x, 6); x++) {
        if ((func_800C587C(D_80147620, 0x4B) ^ 1) != 0) {
            continue;
        }
        map->cells_13[x][y].kind = count | 0x20;
        add_room(&map->rooms_2DC[count++], y, x, y, x);
        func_800BA188(map, x, y, 2, 2);
    }
    x = 1;
    for (y = 2; in_span(y, 4); y++) {
        if ((func_800C587C(D_80147620, 0x4B) ^ 1) != 0) {
            continue;
        }
        map->cells_13[x][y].kind = count | 0x20;
        add_room(&map->rooms_2DC[count++], y, x, y, x);
        func_800BA188(map, x, y, 1, 1);
    }
    x = 6;
    for (y = 2; in_span(y, 4); y++) {
        if ((func_800C587C(D_80147620, 0x4B) ^ 1) != 0) {
            continue;
        }
        map->cells_13[x][y].kind = count | 0x20;
        add_room(&map->rooms_2DC[count++], y, x, y, x);
        func_800BA188(map, x, y, 4, 4);
    }
    for (y = 1; y < 5; y++) {
        for (x = 1; x < 7; x++) {
            func_800B90E8(map, x, y);
        }
    }
    map->room_count_3DC = count;
    func_800B95DC(map);
    D_8014344C = map->room_count_3DC;
    func_800B17A4();
    D_80143392 = 0;
}
