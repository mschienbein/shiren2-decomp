#include "common.h"
typedef unsigned char u8;
typedef struct { s32 x; s32 y; } Pair800B1820;
typedef struct { Pair800B1820 first; Pair800B1820 last; } Rect800B1820;
/* Four per-side bytes at +0x10 (func_80042D84/func_80043228 store them with sb); copied with the record. */
typedef struct { Rect800B1820 bounds; u8 sides_10[4]; } Room800B1820;
typedef struct { Pair800B1820 position; Pair800B1820 first; Pair800B1820 last; } Iter800B1820;
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
extern Pair800B1820 *func_800A3610(Pair800B1820 *out, Iter800B1820 *iter);
/* ODD_C: Small coordinate operations retain the original member-copy and alias boundaries. */
static __inline__ Pair800B1820 *rect_first(Pair800B1820 *p, Rect800B1820 *rect) { p->x = rect->first.x; p->y = rect->first.y; return p; }
static __inline__ Pair800B1820 *rect_last(Pair800B1820 *p, Rect800B1820 *rect) { p->x = rect->last.x; p->y = rect->last.y; return p; }
static __inline__ void begin_room(Iter800B1820 *iter, Room800B1820 *room) {
    Rect800B1820 bounds;
    Pair800B1820 point;
    bounds.first.x = room->bounds.first.x; bounds.first.y = room->bounds.first.y;
    bounds.last.x = room->bounds.last.x; bounds.last.y = room->bounds.last.y;
    iter->first = *rect_first(&point, &bounds);
    iter->position = iter->first;
    iter->last = *rect_last(&point, &bounds);
}
static __inline__ s32 less_equal(s32 a, s32 b) { return !(b < a); }
static __inline__ s32 has_next(Iter800B1820 *iter) { return less_equal(iter->position.x, iter->last.x); }
/* ODD_C: The active-room accessor reads the whole room state object; a direct member
 * read lets CSE share its address with the store and changes allocation. */
static __inline__ Room800B1820 *active_room(RoomState *state) { return state->room; }
/* The original compares the incoming full-width index with signed slt. */
void func_800B1820(s32 removed) {
    if (removed < D_8014344C) {
        s32 next = removed + 1;
        s32 id = removed;
        for (;;) {
            Iter800B1820 iter;
            if (next >= D_8014344C) break;
            D_801431F0[next - 1] = D_801431F0[next];
            begin_room(&iter, &D_801431F0[next]);
            while (has_next(&iter)) {
                Pair800B1820 pos;
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
