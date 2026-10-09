#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    s32 x;
    s32 y;
} Pair;
typedef struct {
    Pair min;
    Pair max;
} Box;
typedef struct {
    Box box;
    u8 pad10[4];
} RoomDef;
typedef struct { u8 value; } Dir;
typedef struct {
    u8 pad0[0xC];
    s32 count;
    s32 index;
    u8 pad14[4];
} BoxIter;
typedef struct {
    u8 pad0[0x3DC];
    s32 roomCount;
    u8 pad3E0[0x578];
    u16 flags;
    u8 pad95A[2];
    s32 pending[16];
    u8 pad99C[0x20];
    Box box;
} Floor;
extern u8 D_80147620[];
extern RoomDef D_801431F0[];
u8 func_800C57CC(void *, s32);
s32 func_800A3138(RoomDef *);
s32 func_800A315C(RoomDef *);
s32 func_800BB22C(Floor *, RoomDef *);
s32 func_800BAFE4(Floor *, RoomDef *);
s32 func_800BD6E8(void *floor, Box *bounds);
void func_800C25F4(BoxIter *, RoomDef *, s32, s32);
void *func_800C2758(void *out, void *iterator);
u32 func_800B1C6C(void *pos);
void *func_800A2594(void *out, void *from, Dir dir);
void func_800B1BE0(Pair *, s32);
void func_800B1820(s32);
static inline s32 canPlaceX(Floor *self, RoomDef *room) {
    return func_800BB22C(self, room) == 1;
}
static inline s32 canPlaceY(Floor *self, RoomDef *room) {
    return func_800BAFE4(self, room) == 1;
}
s32 func_800BDAEC(Floor *self) {
    u8 id;
    RoomDef *room;
    s32 tries;
    s32 width;
    s32 height;
    s32 result;
    s32 side;
    /* The two corners form one rectangle passed to func_800BD6E8; the side
       walk below then reuses them as the cursor and its neighbour. */
    Box corners;
    BoxIter iter;
    Dir dir;
    s32 mark;
    s32 facing;
    s32 blocked;
    Pair *cur;
    Pair *start;
    Pair *adj;
    Dir *pdir;

    if (!(self->flags & 0x80)) {
        return 0;
    }
    id = 0;
    room = 0;
    tries = 100;
    while (1) {
        tries--;
        if (tries == -1) {
            break;
        }
        id = func_800C57CC(D_80147620, (u8)(self->roomCount - 1));
        if (self->pending[id] == 0) {
            continue;
        }
        room = &D_801431F0[id];
        width = func_800A3138(room);
        height = func_800A315C(room);
        if (width >= 0x15) {
            continue;
        }
        if (height >= 0x15) {
            continue;
        }
        if (!(width & 1) && !canPlaceX(self, room)) {
            continue;
        }
        if (!(height & 1) && !canPlaceY(self, room)) {
            continue;
        }
        self->pending[id] = 0;
        break;
    }
    if (tries < 0) {
        return 0;
    }
    self->box = room->box;
    corners.min.x = self->box.min.x;
    start = &corners.min;
    start->y = self->box.min.y;
    corners.max.x = self->box.max.x;
    adj = &corners.max;
    adj->y = self->box.max.y;
    result = func_800BD6E8(self, &corners);
    side = 0;
    while (1) {
        if (side >= 4) {
            break;
        }
        iter.index = 0;
        iter.count = 0;
        func_800C25F4(&iter, room, side, 0);
        facing = (side * 2) & 7;
        while (1) {
            if (iter.index >= iter.count) {
                break;
            }
            func_800C2758(&corners.min, &iter);
            mark = 0;
            cur = &corners.min;
            if (func_800B1C6C(cur) & 0x800) {
                pdir = &dir;
                pdir->value = facing;
                func_800A2594(&corners.max, cur, *pdir);
                blocked = func_800B1C6C(&corners.max) & 0x1000;
                mark = blocked == 0;
            }
            if (mark) {
                func_800B1BE0(cur, 0x800);
            }
        }
        side++;
    }
    func_800B1820(id);
    return result;
}
