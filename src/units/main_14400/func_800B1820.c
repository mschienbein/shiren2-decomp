#include "common.h"
#include "map_position_iterator.h"
typedef unsigned char u8;
/* Four per-side bytes at +0x10 (func_80042D84/func_80043228 store them with sb); copied with the record. */
typedef struct { MapRect bounds; u8 sides_10[4]; } Room800B1820;
/* One of the two 0x18-byte region records initialized by func_800B1080: owner byte +0
 * (func_800D1D90 clears it with sb zero,0(a0)) plus 3 padding bytes, room pointer +4. */
typedef struct { signed char owner_00; u8 pad_01[3]; Room800B1820 *room_04; u8 pad_08[16]; } Link800B1820;
extern Room800B1820 D_801431F0[16];
extern Link800B1820 D_80143330[2];
typedef struct { Room800B1820 *room; s32 opaque[3]; } RoomState;
extern RoomState D_80143434;
extern u8 D_80143448;
extern u8 D_8014344C;
extern unsigned short D_80143450[54][76];
/* ODD_C: Small coordinate operations retain the original member-copy and alias boundaries. */
static __inline__ MapPos *rect_first(MapPos *p, MapRect *rect) { p->x = rect->min.x; p->y = rect->min.y; return p; }
static __inline__ MapPos *rect_last(MapPos *p, MapRect *rect) { p->x = rect->max.x; p->y = rect->max.y; return p; }
static __inline__ void begin_room(MapPositionIterator *iter, Room800B1820 *room) {
    MapRect bounds;
    MapPos point;
    bounds.min.x = room->bounds.min.x; bounds.min.y = room->bounds.min.y;
    bounds.max.x = room->bounds.max.x; bounds.max.y = room->bounds.max.y;
    iter->start = *rect_first(&point, &bounds);
    iter->cur = iter->start;
    iter->end = *rect_last(&point, &bounds);
}
static __inline__ s32 less_equal(s32 a, s32 b) { return !(b < a); }
static __inline__ s32 has_next(MapPositionIterator *iter) { return less_equal(iter->cur.x, iter->end.x); }
/* ODD_C: The active-room accessor reads the whole room state object; a direct member
 * read lets CSE share its address with the store and changes allocation. */
static __inline__ Room800B1820 *active_room(RoomState *state) { return state->room; }
/* The original compares the incoming full-width index with signed slt. */
void func_800B1820(s32 removed) {
    if (removed < D_8014344C) {
        s32 next = removed + 1;
        s32 id = removed;
        for (;;) {
            MapPositionIterator iter;
            if (next >= D_8014344C) break;
            D_801431F0[next - 1] = D_801431F0[next];
            begin_room(&iter, &D_801431F0[next]);
            while (has_next(&iter)) {
                MapPos pos;
                func_800A3610(&pos, &iter);
                D_80143450[pos.x][pos.y] &= 0xFFF0;
                D_80143450[pos.x][pos.y] |= id & 15;
            }
            id++;
            next++;
        }
        {
            s32 i = 0;
            Room800B1820 *boundary = &D_801431F0[removed];
            /* One room cursor serves the link table and the active room. */
            Room800B1820 *room;
            D_8014344C--;
            for (; ; i++) {
                s32 count = D_80143448;
                Link800B1820 *link;
                if (i >= count) break;
                link = &D_80143330[i];
                room = link->room_04;
                if (room != 0 && room > boundary) {
                    room--;
                    link->room_04 = room;
                }
            }
            room = active_room(&D_80143434);
            if (room != 0 && room > &D_801431F0[removed]) {
                room--;
                D_80143434.room = room;
            }
        }
    }
}
