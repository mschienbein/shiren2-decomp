#include "common.h"
#include "map_line_iterator.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05; unsigned short field_06; unsigned char mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;

typedef unsigned char u8;
typedef unsigned short u16;

/* Room rectangle followed by the per-side exit counts. */
typedef struct { MapRect rect; u8 exits[4]; } Room800B4098;

/* Game mode byte; the top three bits select the floor mode. */



void func_800A3180(MapRect *rect);
u32 func_800B1C6C(MapPos *pos);
void func_800B1B58(MapPos *pos, u16 flags);

static inline void edge_iter_init(MapLineIterator *it)
{
    it->index = 0;
    it->length = 0;
}

static inline s32 edge_iter_more(MapLineIterator *it)
{
    return it->index < it->length;
}

/* Sets the room rectangle and counts the exit cells along each of its four sides. */
void func_800B6590(Room800B4098 *room, MapPos *tl, MapPos *br)
{
    MapLineIterator it;
    MapPos exit;
    MapPos cell;
    s32 marked;

    room->rect.min = *tl;
    room->rect.max = *br;
    func_800A3180(&room->rect);
    marked = (D_80142F18.mode & 0xE0) == 64;
    if (marked) {
        s32 side;

        edge_iter_init(&it);
        for (side = 0;; side++) {
            MapPos *pos = &exit;

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
            MapPos *pos = &cell;

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
