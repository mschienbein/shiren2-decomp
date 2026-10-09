#include "common.h"
typedef unsigned char u8;
typedef struct { s32 x, y; } Position;
typedef struct { s32 left, top, right, bottom; u8 flags10; u8 pad11[3]; } Room;
typedef struct { Position position; u8 direction; s32 length, current; } Line;
extern Room D_801431F0[];
extern void func_800C25D0(Line *, Position *, u8 *, s32);
extern void *func_800C2758(void *, void *);
extern u32 func_800B1C6C(Position *);
extern s32 func_800B5B60(void *);
extern s32 func_800B2048(void *);
/* ODD_C: loop-test accessor; it also keeps GCC from duplicating the exit test. */
static inline s32 has_next(Line *line)
{
    return line->current < line->length;
}
s32 func_800B3DF0(u8 first, u8 second)
{
    Position start;
    Line line;
    Position point;
    u8 directionByte;
    Position *p;
    s32 direction = 0;
    s32 length = 0, found = 0, blocked, both;
    Room *a = &D_801431F0[first];
    Room *b = &D_801431F0[second];
    s32 topA = a->top, leftA = a->left, bottomA = a->bottom, rightA = a->right;
    s32 topB = b->top, leftB = b->left, bottomB = b->bottom, rightB = b->right;
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
