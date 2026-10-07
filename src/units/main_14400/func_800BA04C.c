#include "common.h"

typedef unsigned char u8;

typedef struct { s32 row; s32 col; } Point;
typedef struct { Point start; Point end; } Rect;
typedef struct { u8 kind; u8 attr; u8 pad2[2]; } Cell;
typedef struct {
    u8 pad0[0x13];
    Cell cells[7][8];
    u8 pad[0x2DC - 0x13 - 7 * 8 * 4];
    Rect rects[16];
    s32 rectCount;
} Board;
static inline void copyRect(Rect *dst, Rect *src) {
    dst->start = src->start;
    dst->end = src->end;
}

void func_800B90E8(Board *board, u8 col, u8 row);
void func_800BA04C(Board *board, u8 kinds[][6], u8 attrs[][6]) {
    u8 count = 0;
    s32 row;
    s32 col;
    Rect copy;
    Rect rect;
    row = 1;
    while (1) {
        if (row >= 5) {
            break;
        }
        col = 1;
        while (1) {
            u8 kind;
            if (col >= 7) {
                break;
            }
            kind = kinds[row - 1][col - 1];
            board->cells[col][row].kind = kind;
            if (kind & 0x20) {
                count++;
                rect.start.row = row;
                rect.start.col = col;
                rect.end.row = row;
                rect.end.col = col;
                copy.start = rect.start;
                copy.end = rect.end;
                copyRect(&board->rects[kind & 0xF], &copy);
            }
            board->cells[col][row].attr = attrs[row - 1][col - 1];
            func_800B90E8(board, col, row);
            col++;
        }
        row++;
    }
    board->rectCount = count;
}
