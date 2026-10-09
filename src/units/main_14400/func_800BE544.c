#include "common.h"

typedef struct { s32 x, y; } Pos;
typedef struct { unsigned char room; unsigned char flags; unsigned char pad[2]; } Cell;
typedef struct {
    char pad0[0x10];
    unsigned char rows;
    unsigned char cols;
    char pad12;
    Cell grid[11][8];
    char pad173[0x3F4 - 0x173];
    s32 x3F4;
    char pad3F8[0x958 - 0x3F8];
    unsigned short x958;
    char pad95A[2];
    s32 x95C[16];
} Map;
typedef struct { s32 x0, x4, x8, xC; unsigned char pad10[4]; } Room;
extern Room D_801431F0[];
extern char D_80147620[];
unsigned char func_800C57CC(void *, s32);
s32 func_800C5844(void *, unsigned char, unsigned char);
u32 func_800B1C6C(Pos *);
s32 func_800B1AB8(Pos *);
void func_800BE1B8(Map *, Pos *, Pos *, s32, s32);
unsigned char func_800C57A0(void *);
void *func_800AC244(unsigned char);
s32 func_800AE2A4(void *, s32, s32, Room *);
s32 func_800BE544(Map *map) {
    s32 ok[4];
    s32 row[4];
    s32 col[4];
    Pos a, b, c, d;
    Room *rm;
    s32 i;
    s32 vert;
    unsigned char side;
    s32 tries;
    s32 x0, x4, x8, xC;
    if (!(map->x958 & 0x200)) return 0;
    if (map->x3F4 == 0) return 0;
    for (i = 3; i >= 0; i--) ok[i] = 0;
    /* ODD_C: in each side check below, the far-neighbour test is written as
       two arms that both set the flag (a non-room neighbour, or a different
       room); GCC cross-jumps them as in the original instead of materializing
       a combined || predicate. */
    /* ODD_C: capture the unsigned dimension before storing the edge; this
       preserves its variable-shift evaluation rather than folding the shift. */
    {
        u32 extent = map->rows;
        col[0] = 1;
        row[0] = (extent >> col[0]) + col[0];
    }
    {
        unsigned char room = map->grid[row[0]][col[0]].room;
        if (room & 0x20) {
            unsigned char key = room & 0xF;
            s32 above = 0, below = 0;
            if (map->grid[row[0]][col[0]].flags & 4) {
                unsigned char o = map->grid[row[0] - 1][col[0]].room;
                if (o & 0x20) above = key != (o & 0xF);
                else above = 1;
            }
            if (map->grid[row[0]][col[0]].flags & 1) {
                unsigned char o = map->grid[row[0] + 1][col[0]].room;
                if (!(o & 0x20)) below = 1;
                else if ((o & 0xF) != key) below = 1;
            }
            {
                s32 v = 0;
                if (above && below) v = map->x95C[key] != 0;
                ok[0] = v;
            }
        }
    }
    row[1] = (map->rows >> 1) + 1;
    col[1] = map->cols;
    {
        unsigned char room = map->grid[row[1]][col[1]].room;
        if (room & 0x20) {
            unsigned char key = room & 0xF;
            s32 above = 0, below = 0;
            if (map->grid[row[1]][col[1]].flags & 4) {
                unsigned char o = map->grid[row[1] - 1][col[1]].room;
                if (o & 0x20) above = key != (o & 0xF);
                else above = 1;
            }
            if (map->grid[row[1]][col[1]].flags & 1) {
                unsigned char o = map->grid[row[1] + 1][col[1]].room;
                if (!(o & 0x20)) below = 1;
                else if ((o & 0xF) != key) below = 1;
            }
            {
                s32 v = 0;
                if (above && below) v = map->x95C[key] != 0;
                ok[1] = v;
            }
        }
    }
    row[2] = 1;
    col[2] = (map->cols >> 1) + 1;
    {
        unsigned char room = map->grid[row[2]][col[2]].room;
        if (room & 0x20) {
            unsigned char key = room & 0xF;
            s32 left = 0, right = 0;
            if (map->grid[row[2]][col[2]].flags & 2) {
                unsigned char o = map->grid[row[2]][col[2] - 1].room;
                if (o & 0x20) left = key != (o & 0xF);
                else left = 1;
            }
            if (map->grid[row[2]][col[2]].flags & 8) {
                unsigned char o = map->grid[row[2]][col[2] + 1].room;
                if (!(o & 0x20)) right = 1;
                else if ((o & 0xF) != key) right = 1;
            }
            {
                s32 v = 0;
                if (left && right) v = map->x95C[key] != 0;
                ok[2] = v;
            }
        }
    }
    row[3] = map->rows;
    col[3] = (map->cols >> 1) + 1;
    {
        unsigned char room = map->grid[row[3]][col[3]].room;
        if (room & 0x20) {
            unsigned char key = room & 0xF;
            s32 left = 0, right = 0;
            if (map->grid[row[3]][col[3]].flags & 2) {
                unsigned char o = map->grid[row[3]][col[3] - 1].room;
                if (o & 0x20) left = key != (o & 0xF);
                else left = 1;
            }
            if (map->grid[row[3]][col[3]].flags & 8) {
                unsigned char o = map->grid[row[3]][col[3] + 1].room;
                if (!(o & 0x20)) right = 1;
                else if ((o & 0xF) != key) right = 1;
            }
            {
                s32 v = 0;
                if (left && right) v = map->x95C[key] != 0;
                ok[3] = v;
            }
        }
    }
    if (!ok[0] && !ok[1] && !ok[2] && !ok[3]) return 0;
    do {
        side = func_800C57CC(D_80147620, 3);
    } while (ok[side] == 0);
    /* ODD_C: each retry below consumes a fresh decrement result at the loop
       head; this keeps the measured predecrement guard instead of rotating
       the loop. The retry budget is set up with the defaults, before the
       chosen side's room is looked up. */
    tries = 10;
    vert = 1;
    {
        s32 key = map->grid[row[side]][col[side]].room & 0xF;
        map->x95C[key] = 0;
        rm = &D_801431F0[key];
    }
    x4 = rm->x4;
    x0 = rm->x0;
    xC = rm->xC;
    x8 = rm->x8;
    switch (side) {
    case 0:
        a.x = x8 + 1;
        for (;;) {
            s32 remaining = --tries;
            if (remaining == -1) break;
            a.y = func_800C5844(D_80147620, (x4 + 1) & 0xFF, (xC - 1) & 0xFF) & 0xFF;
            if (!(func_800B1C6C(&a) & 0x800)) break;
        }
        if (tries < 0) return 0;
        if ((func_800B1AB8(&a) ^ 1) != 0) a.x--;
        b.x = x0 - 1;
        b.y = func_800C5844(D_80147620, (x4 + 1) & 0xFF, (xC - 1) & 0xFF) & 0xFF;
        if ((func_800B1AB8(&b) ^ 1) != 0) b.x++;
        c.x = 10;
        d.x = 0x2B;
        c.y = b.y;
        d.y = func_800C5844(D_80147620, 10, 0x41) & 0xFF;
        vert = 0;
        break;
    case 1:
        a.x = x0 - 1;
        for (;;) {
            s32 remaining = --tries;
            if (remaining == -1) break;
            a.y = func_800C5844(D_80147620, (x4 + 1) & 0xFF, (xC - 1) & 0xFF) & 0xFF;
            if (!(func_800B1C6C(&a) & 0x800)) break;
        }
        if (tries < 0) return 0;
        if ((func_800B1AB8(&a) ^ 1) != 0) a.x++;
        b.x = x8 + 1;
        b.y = func_800C5844(D_80147620, (x4 + 1) & 0xFF, (xC - 1) & 0xFF) & 0xFF;
        if ((func_800B1AB8(&b) ^ 1) != 0) b.x--;
        c.x = 0x2B;
        d.x = 10;
        c.y = b.y;
        d.y = func_800C5844(D_80147620, 10, 0x41) & 0xFF;
        vert = 0;
        break;
    case 2:
        a.y = xC + 1;
        for (;;) {
            s32 remaining = --tries;
            if (remaining == -1) break;
            a.x = func_800C5844(D_80147620, (x0 + 1) & 0xFF, (x8 - 1) & 0xFF) & 0xFF;
            if (!(func_800B1C6C(&a) & 0x800)) break;
        }
        if (tries < 0) return 0;
        if ((func_800B1AB8(&a) ^ 1) != 0) a.y--;
        b.y = x4 - 1;
        b.x = func_800C5844(D_80147620, (x0 + 1) & 0xFF, (x8 - 1) & 0xFF) & 0xFF;
        if ((func_800B1AB8(&b) ^ 1) != 0) b.y++;
        c.y = 10;
        d.y = 0x41;
        c.x = b.x;
        d.x = func_800C5844(D_80147620, 10, 0x2B) & 0xFF;
        break;
    case 3:
        a.y = x4 - 1;
        for (;;) {
            s32 remaining = --tries;
            if (remaining == -1) break;
            a.x = func_800C5844(D_80147620, (x0 + 1) & 0xFF, (x8 - 1) & 0xFF) & 0xFF;
            if (!(func_800B1C6C(&a) & 0x800)) break;
        }
        if (tries < 0) return 0;
        if ((func_800B1AB8(&a) ^ 1) != 0) a.y++;
        b.y = xC + 1;
        b.x = func_800C5844(D_80147620, (x0 + 1) & 0xFF, (x8 - 1) & 0xFF) & 0xFF;
        if ((func_800B1AB8(&b) ^ 1) != 0) b.y--;
        c.y = 0x41;
        d.y = 10;
        c.x = b.x;
        d.x = func_800C5844(D_80147620, 10, 0x2B) & 0xFF;
        break;
    }
    func_800BE1B8(map, &a, &b, vert, 1);
    func_800BE1B8(map, &b, &c, vert, 0);
    func_800BE1B8(map, &a, &d, vert, 0);
    if (func_800C57A0(D_80147620) & 1) {
        void *fx = func_800AC244(0xD7);
        if (fx != 0) func_800AE2A4(fx, 0, 0, rm);
    }
    return 1;
}
