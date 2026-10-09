#include "common.h"

/*
 * Fixed grid floor preset (C++ TU). Board::setup is the shared inline that
 * clamps its (modified) size parameters, selects the edge tables and runs
 * func_800B8628; the clamped parameters are what keep the 2 / 7 / 5 values in
 * registers (the sllv by a1 and the andi of the stored sizes).
 */

typedef unsigned char u8;
typedef signed char s8;

struct Point {
    s32 x;
    s32 y;
};

struct Rect {
    Point a;
    Point b;
    Rect() {}
    Rect(s32 x0, s32 y0, s32 x1, s32 y1) { a.x = x0; a.y = y0; b.x = x1; b.y = y1; }
    Rect(const Rect &o) : a(o.a), b(o.b) {}
    Rect &operator=(const Rect &o)
    {
        a = o.a;
        b = o.b;
        return *this;
    }
};

struct Cell {
    u8 kind;
    u8 attr;
    u8 pad2[2];
};

extern "C" {
/* Per-size edge tables: D_80153BDC[rows - 3] and D_80153BF8[cols - 3]. */
extern s8 *D_80153BDC[];
extern s8 *D_80153BF8[];
extern u8 D_8014344C;
extern u8 D_80143392;
}

/* Partial view of the grid floor generator. */
struct Board {
    u8 pad0[0x10];
    u8 grid_rows;
    u8 grid_cols;
    u8 field_12;
    /* ROM 0x8B6C8..0x8B6D0: 4-byte cells at +0x13, words at +0x174.
     * Both grids have 11 rows of 8; the edge pointers follow at +0x2D4. */
    Cell cells[11][8];
    u8 pad173;
    s32 available[11][8];
    s8 *row_edges;
    s8 *col_edges;
    Rect rects[16];
    s32 room_count;
    void setup(u8 rows, u8 cols, u8 extra, u8 mode);
};

extern "C" {
void func_800B8628(Board *board, u8 mode);
void func_800B90E8(Board *board, u8 col, u8 row);
void func_800B95DC(Board *board);
void func_800B17A4(void);
}

/* Clamps the grid size to 3..8 x 3..5, picks the edge tables, then lays out the cells. */
inline void Board::setup(u8 rows, u8 cols, u8 extra, u8 mode)
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
    grid_rows = rows;
    grid_cols = cols;
    s8 *row_table = D_80153BDC[grid_rows - 3];
    s8 *col_table = D_80153BF8[grid_cols - 3];
    row_edges = row_table;
    col_edges = col_table;
    field_12 = extra;
    func_800B8628(this, mode);
}

extern "C" {
extern u8 D_80153E64[5][7];
extern u8 D_80153E88[5][7];
extern u8 D_80153EAC[];
extern u8 D_80153EB0[];
extern u8 D_80153EB4[];
extern u8 D_80153EB8[];
}

static inline Rect make_rect(s32 x0, s32 y0, s32 x1, s32 y1)
{
    Rect r(x0, y0, x1, y1);
    return r;
}

extern "C" void func_800C1970(Board *board)
{
    board->setup(7, 5, 4, 2);
    for (s32 row = 1; row < 6; row++) {
        for (s32 col = 1; col < 8; col++) {
            u8 kind = D_80153E64[row - 1][col - 1];
            board->cells[col][row].kind = kind;
            if (kind & 0x20) {
                kind &= 0xF;
                board->rects[kind] = make_rect(D_80153EB4[kind], D_80153EAC[kind], D_80153EB8[kind], D_80153EB0[kind]);
            }
            board->cells[col][row].attr = D_80153E88[row - 1][col - 1];
            func_800B90E8(board, col, row);
        }
    }
    board->room_count = 4;
    func_800B95DC(board);
    D_8014344C = board->room_count;
    func_800B17A4();
    D_80143392 = 0;
}
