#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x, y; } Position;
typedef struct { Position first, last; } Bounds;
typedef struct {
    s32 left, top, right, bottom;
    struct { u8 unused[3], value; } side[4];
} Room;
typedef struct { u8 unknown00[15]; u8 count; Room *room; } Floor;
typedef struct { Floor *floor; } Context;
typedef struct { Bounds bounds; u8 side[4]; } RoomInfo;
typedef struct { s32 unknown00[3]; s32 field0c; } Object;
extern u8 D_8014344C;
/* The 54 by 76 cell map (0x1008 cells). */
extern u16 D_80143450[54][76];
extern RoomInfo D_801431F0[];
extern Context *func_80066B20(void);
extern void func_80066B2C(void *map, s32 floor_id);
extern void func_800B17A4(void);
extern s32 func_80049CB4(s32 id, ...);
static inline void copy_bounds(Bounds *target, Bounds *source) {
    target->first = source->first;
    target->last = source->last;
}
/* Marks every room's perimeter cells, records each room's bounds and the
 * number of open perimeter cells per side, then clears the scratch marks. */
void func_80043228(Object *object) {
    Bounds bounds;
    Position *start;
    Context *context = func_80066B20();
    s32 room_index;
    u16 *cell;
    s32 index;
    func_80066B2C(D_80143450, object->field0c);
    room_index = 0;
    start = &bounds.first;
    D_8014344C = context->floor->count;
    for (;;) {
        Room *room;
        s32 side;
        s32 first_y, first_x, last_y, last_x;
        if (room_index >= D_8014344C) break;
        room = &context->floor->room[room_index];
        first_y = room->top;
        first_x = room->left;
        bounds.first.x = first_y;
        start->y = first_x;
        last_y = room->bottom;
        last_x = room->right;
        bounds.last.x = last_y;
        bounds.last.y = last_x;
        copy_bounds(&D_801431F0[room_index].bounds, &bounds);
        side = 0;
        for (;;) {
            s32 x1, x2, y1, y2, x, y, count, flags;
            if (side >= 4) break;
            switch (side) {
            case 1: x1 = room->left; y2 = room->top - 1; x2 = room->right; y1 = y2; break;
            case 3: x1 = room->left; y2 = room->bottom + 1; x2 = room->right; y1 = y2; break;
            case 2: y1 = room->top - 1; x1 = x2 = room->left - 1; y2 = room->bottom + 1; break;
            case 0: default: y1 = room->top - 1; x1 = x2 = room->right + 1; y2 = room->bottom + 1; break;
            }
            count = 0;
            for (y = y1; y <= y2; y++) {
                x = x1;
                while (x <= x2) {
                    flags = D_80143450[y][x];
                    if ((flags & 0x800) && (flags & 0xe100)) {
                        D_80143450[y][x] |= 0x80;
                        count++;
                    }
                    x++;
                }
            }
            D_801431F0[room_index].side[side] = room->side[side].value - count;
            side++;
        }
        room_index++;
    }
    cell = (u16 *)D_80143450;
    index = 0;
    do {
        if (*cell & 0x80) *cell &= 0xf77f;
        index++;
        cell++;
    } while (index < 0x1008);
    func_800B17A4();
    func_80049CB4(5, 1);
}
