#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct { s32 x; s32 y; } Pos800B6590;
typedef struct { Pos800B6590 tl; Pos800B6590 br; } Rect800B6590;
/* Room rectangle followed by the per-side exit counts. */
typedef struct { Rect800B6590 rect; u8 exits[4]; } Room800B4098;
typedef struct { u8 pad0[0xC]; s32 count; s32 index; } EdgeIter800B6590;

/* Game mode byte; the top three bits select the floor mode. */



void func_800A3180(Rect800B6590 *rect);
void func_800C25F4(EdgeIter800B6590 *it, Rect800B6590 *rect, s32 side, s32 arg3);
void *func_800C2758(void *out, void *iterator);
u32 func_800B1C6C(Pos800B6590 *pos);
void func_800B1B58(Pos800B6590 *pos, u16 flags);

static inline void edge_iter_init(EdgeIter800B6590 *it)
{
    it->index = 0;
    it->count = 0;
}

static inline s32 edge_iter_more(EdgeIter800B6590 *it)
{
    return it->index < it->count;
}

/* Sets the room rectangle and counts the exit cells along each of its four sides. */
void func_800B6590(Room800B4098 *room, Pos800B6590 *tl, Pos800B6590 *br)
{
    EdgeIter800B6590 it;
    Pos800B6590 exit;
    Pos800B6590 cell;
    s32 marked;

    room->rect.tl = *tl;
    room->rect.br = *br;
    func_800A3180(&room->rect);
    marked = (D_80142F18.mode & 0xE0) == 64;
    if (marked) {
        s32 side;

        edge_iter_init(&it);
        for (side = 0;; side++) {
            Pos800B6590 *pos = &exit;

            if (side >= 4) {
                break;
            }
            room->exits[side] = 0;
            func_800C25F4(&it, &room->rect, side, 1);
            while (edge_iter_more(&it)) {
                func_800C2758(pos, &it);
                if (func_800B1C6C(pos) & 0x800) {
                    room->exits[side]++;
                }
            }
        }
    } else {
        s32 side;

        edge_iter_init(&it);
        for (side = 0;; side++) {
            Pos800B6590 *pos = &cell;

            if (side >= 4) {
                break;
            }
            room->exits[side] = 0;
            func_800C25F4(&it, &room->rect, side, 0);
            while (edge_iter_more(&it)) {
                func_800C2758(pos, &it);
                if (!(func_800B1C6C(pos) & 0xE100)) {
                    func_800B1B58(pos, 0x800);
                    room->exits[side]++;
                }
            }
        }
    }
}
