#include "common.h"

typedef struct {
    s32 x, y;
} Pos800B6768;

/* Room rectangle at the start of the 0x14-byte room record (exit counts follow). */
typedef struct {
    Pos800B6768 topLeft;
    Pos800B6768 bottomRight;
} Room800B6768;

u32 func_800B1C6C(Pos800B6768 *pos);
void func_800B6590(Room800B6768 *room, Pos800B6768 *topLeft, Pos800B6768 *bottomRight);

/* ODD_C: midpoint helper; its inlined parameter copies also keep the corner loads in scratch
 * registers ahead of the side variables. */
static inline s32 middle(s32 lo, s32 hi) { return lo + (hi - lo) / 2; }
static inline void set_pos(Pos800B6768 *pos, s32 x, s32 y) { pos->x = x; pos->y = y; }

/* Grow a rectangle from the room centre up, left, down and right while the next tile still
 * carries flag 0x1000, then pass the resulting corners to func_800B6590. */
void func_800B6768(Room800B6768 *room)
{
    Pos800B6768 probe, bottomRight;
    s32 top, left, bottom, right;

    bottom = middle(room->topLeft.y, room->bottomRight.y);
    right = middle(room->topLeft.x, room->bottomRight.x);
    top = bottom;
    left = right;
    for (;;) {
        s32 next = top - 1;
        set_pos(&probe, right, next);
        if (!(func_800B1C6C(&probe) & 0x1000)) break;
        top = next;
    }
    for (;;) {
        s32 next = left - 1;
        set_pos(&probe, next, top);
        if (!(func_800B1C6C(&probe) & 0x1000)) break;
        left = next;
    }
    for (;;) {
        s32 next = bottom + 1;
        set_pos(&probe, right, next);
        if (!(func_800B1C6C(&probe) & 0x1000)) break;
        bottom = next;
    }
    for (;;) {
        s32 next = right + 1;
        set_pos(&probe, next, bottom);
        if (!(func_800B1C6C(&probe) & 0x1000)) break;
        right = next;
    }
    set_pos(&probe, left, top);
    set_pos(&bottomRight, right, bottom);
    func_800B6590(room, &probe, &bottomRight);
}
