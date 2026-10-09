#include "common.h"

/*
 * Fixed grid floor preset (C++ TU). Board::setup is the shared inline that
 * clamps its (modified) size parameters, selects the edge tables and runs
 * func_800B8628; the clamped parameters are what keep the 2 / 7 / 5 values in
 * registers (the sllv by a1 and the andi of the stored sizes).
 */

typedef unsigned char u8;
typedef signed char s8;

extern "C" {
/* Per-size edge tables: D_80153BDC[rows - 3] and D_80153BF8[cols - 3]. */
extern s8 *D_80153BDC[];
extern s8 *D_80153BF8[];
extern u8 D_8014344C;
extern u8 D_80143392;
}

struct Cell { u8 kind, attr, x, y; };

/* Partial view of the grid floor generator. */
struct Board {
    u8 pad0[0x10];
    u8 grid_rows;
    u8 grid_cols;
    u8 field_12;
    /* Complete grids shared with func_800B8628, including border cells. */
    Cell cells[11][8];
    u8 pad173;
    s32 available[11][8];
    s8 *row_edges;
    s8 *col_edges;
    u8 pad2DC[0x3DC - 0x2DC];
    s32 room_count;
    void setup(u8 rows, u8 cols, u8 extra, u8 mode);
};

extern "C" {
void func_800B8628(Board *board, u8 mode);
void func_800BA04C(Board *board, u8 kinds[][6], u8 attrs[][6]);
void func_800BA188(void *map, u8 x, u8 y, s32 dir_a, s32 dir_b);
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
extern u8 D_80153E30[][6];
extern u8 D_80153E48[][6];
}

extern "C" void func_800C1780(Board *board)
{
    board->setup(6, 4, 8, 2);
    func_800BA04C(board, D_80153E30, D_80153E48);
    func_800BA188(board, 2, 2, 2, 4);
    func_800BA188(board, 3, 2, 2, 4);
    func_800BA188(board, 3, 2, 8, 1);
    func_800BA188(board, 4, 2, 2, 1);
    func_800BA188(board, 4, 2, 8, 4);
    func_800BA188(board, 5, 2, 2, 1);
    func_800BA188(board, 2, 3, 8, 4);
    func_800BA188(board, 3, 3, 8, 4);
    func_800BA188(board, 3, 3, 2, 1);
    func_800BA188(board, 4, 3, 8, 1);
    func_800BA188(board, 4, 3, 2, 4);
    func_800BA188(board, 5, 3, 8, 1);
    func_800B95DC(board);
    D_8014344C = board->room_count;
    func_800B17A4();
    D_80143392 = 0;
}
