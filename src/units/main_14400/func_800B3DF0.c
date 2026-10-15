#include "common.h"
#include "map_line_iterator.h"
typedef unsigned char u8;
/* Foreign room-table view: geometry plus four side-metadata bytes, 20 bytes total. */
typedef struct { MapRect bounds; u8 flags10, flags11, flags12, flags13; } Room;
extern Room D_801431F0[];
extern u32 func_800B1C6C(MapPos *);
extern s32 func_800B5B60(void *);
extern s32 func_800B2048(void *);
/* ODD_C: loop-test accessor; it also keeps GCC from duplicating the exit test. */
static inline s32 has_next(MapLineIterator *line)
{
    return line->index < line->length;
}
s32 func_800B3DF0(u8 first, u8 second)
{
    MapPos start;
    MapLineIterator line;
    MapPos point;
    u8 directionByte;
    MapPos *p;
    s32 direction = 0;
    s32 length = 0, found = 0, blocked, both;
    Room *a = &D_801431F0[first];
    Room *b = &D_801431F0[second];
    s32 topA = a->bounds.min.y, leftA = a->bounds.min.x, bottomA = a->bounds.max.y, rightA = a->bounds.max.x;
    s32 topB = b->bounds.min.y, leftB = b->bounds.min.x, bottomB = b->bounds.max.y, rightB = b->bounds.max.x;
    /* Same columns: probe the one-row gap between vertically adjacent rooms (direction 6). */
    if (leftA == leftB && rightA == rightB) {
        start.x = leftA;
        direction = 6;
        length = rightB - leftA + 1;
        if (bottomA + 1 == topB - 1) { start.y = bottomA + 1; found = 1; }
        if (bottomB + 1 == topA - 1) { start.y = bottomB + 1; found = 1; }
    }
    /* Same rows: probe the one-column gap between horizontally adjacent rooms (direction 0). */
    if (topA == topB && bottomA == bottomB) {
        start.y = topA;
        direction = 0;
        length = bottomA - topA + 1;
        if (rightA + 1 == leftB - 1) { start.x = rightA + 1; found = 1; }
        if (rightB + 1 == leftA - 1) { start.x = rightB + 1; found = 1; }
    }
    if (!found) return 0;
    directionByte = direction;
    func_800C25D0(&line, &start, &directionByte, length);
    p = &point;
    while (has_next(&line)) {
        func_800C2758(p, &line);
        blocked = (func_800B1C6C(p) & 0x4000) ||
            (func_800B1C6C(p) & 0x2000) || (func_800B1C6C(p) & 0x80);
        if (blocked) return 0;
    }
    both = (func_800B5B60(a) || func_800B2048(a)) &&
        (func_800B5B60(b) || func_800B2048(b));
    return both ^ 1;
}
