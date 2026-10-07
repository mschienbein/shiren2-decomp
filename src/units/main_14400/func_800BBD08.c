#include "common.h"
typedef struct { s32 x, y; } Point;
typedef unsigned char u8;
typedef struct { u8 value; } Dir;
typedef struct { s32 x0, y0, x1, y1; unsigned char sides[4]; } Room;
typedef struct { char pad[0x3DC]; s32 roomCount; } Map;
extern Room D_801431F0[];
extern void *func_800B6A98(void *out, void *room, s32 index);
extern void *func_800A25D8(void *out, void *from, Dir dir, s32 scale);
extern void *func_800A2594(void *out, void *from, Dir dir);
extern void func_800A2758(void *pos, Dir dir);
extern s32 func_800B69F4(Room *, Point *), func_801368E4(Point *), func_800B68F0(Room *, Point *);
extern u32 func_800B1C6C(void *pos);
static inline Dir *direction(Dir *out, s32 value) { out->value = value & 7; return out; }
static inline s32 isZero(s32 value) { return value == 0; }
static inline void copyPoint(Point *out, const Point *in) { out->x = in->x; out->y = in->y; }
static inline s32 passable(Point *p) {
    s32 result = 0;
    if (func_801368E4(p) && !(func_800B1C6C(p) & 0x1000)) result = isZero(func_800B1C6C(p) & 0x800);
    return result;
}
s32 func_800BBD08(Map *map, Room *room, s32 index) {
    Point start, step, neighbor, turn;
    Dir d0, d1, d2, d3, d4, d5, d6, d7, d8, d9, d10, d11;
    s32 side;
    unsigned char heading, first, initial;
    s32 turned, found, remaining;
    func_800B6A98(&start, room, index);
    side = func_800B69F4(room, &start);
    if (room->sides[side] != 1) return 0;
    { s32 bound = room->y0; if (start.y == bound) return 0; }
    { s32 bound = room->y1; if (start.y == bound) return 0; }
    { s32 bound = room->x0; if (start.x == bound) return 0; }
    { s32 bound = room->x1; if (start.x == bound) return 0; }
    heading = side * 2;
    { Point *p = &step; s32 blocked;
      func_800A25D8(p, &start, *direction(&d0, heading), 1);
      blocked = passable(p) != 1; if (blocked) return 0; }
    { Point *p = &step; s32 blocked;
      func_800A25D8(p, &start, *direction(&d1, heading), 2);
      blocked = passable(p) != 1; if (blocked) return 0; }
    first = heading; initial = heading; turned = 0; found = 0; remaining = 100;
    copyPoint(&step, &start);
    for (;;) {
        s32 count = --remaining;
        if (count == -1) break;
        {
            s32 current = heading;
            signed char straight = current & 7, right;
            s32 exits;
            func_800A2758(&step, *direction(&d2, straight));
            if (func_800B1C6C(&step) & 0x800) { found = 1; break; }
            func_800A2594(&neighbor, &step, *direction(&d3, straight));
            exits = isZero(func_800B1C6C(&neighbor) & 0xE100);
            right = (current + 2) & 7;
            func_800A2594(&neighbor, &step, *direction(&d4, right));
            if (!(func_800B1C6C(&neighbor) & 0xE100)) exits++;
            func_800A2594(&neighbor, &step, *direction(&d5, current + 4));
            if (!(func_800B1C6C(&neighbor) & 0xE100)) exits++;
            func_800A2594(&neighbor, &step, *direction(&d6, current + 6));
            if (!(func_800B1C6C(&neighbor) & 0xE100)) exits++;
            if (exits != 2) break;
            func_800A2594(&neighbor, &step, *direction(&d7, straight));
            if (func_800B1C6C(&neighbor) & 0xE100) {
                func_800A2594(&turn, &step, *direction(&d8, right));
                if (!(func_800B1C6C(&turn) & 0xE100)) heading += 2;
                func_800A2594(&turn, &step, *direction(&d9, heading + 6));
                if (!(func_800B1C6C(&turn) & 0xE100)) heading += 6;
                heading &= 7;
                if (!turned) { turned = 1; first = heading; }
            }
            { unsigned char difference;
              if (initial >= heading) difference = initial - heading; else difference = heading - initial;
              if (difference == 4) break;
              if (turned) {
                  if (first >= heading) difference = first - heading; else difference = heading - first;
                  if (difference == 4) break;
              }
            }
        }
    }
    if (!found) return 0;
    {
        unsigned char index;
        Room *destination;
        copyPoint(&neighbor, &step);
        for (index = 0; index < map->roomCount; index++) if (func_800B68F0(&D_801431F0[index], &neighbor)) break;
        destination = &D_801431F0[index];
        side = func_800B69F4(destination, &neighbor);
        if (destination->sides[side] != 1) return 0;
        { s32 bound = destination->y0; if (neighbor.y == bound) return 0; }
        { s32 bound = destination->y1; if (neighbor.y == bound) return 0; }
        { s32 bound = destination->x0; if (neighbor.x == bound) return 0; }
        { s32 bound = destination->x1; if (neighbor.x == bound) return 0; }
        { Point *p = &turn; s32 blocked;
          func_800A25D8(p, &neighbor, *direction(&d10, side * 2), 1);
          blocked = passable(p) != 1; if (blocked) return 0; }
        { Point *p = &turn;
          func_800A25D8(p, &neighbor, *direction(&d11, side * 2), 2);
          return passable(p); }
    }
}
