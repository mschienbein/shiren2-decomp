#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x, y; } Coord;
typedef struct { s32 values[4]; } Bounds;
typedef struct { u8 pad0[0x1C]; u16 flags; } Object;
extern void *func_800B1F90(void *);
extern s32 *func_800B1F58(s32 *);
extern Coord *func_800A33DC(Coord *, Bounds *);
extern void *func_800B221C(Coord *);
extern s32 func_800A4314(Object *, Coord *);
extern u32 func_800B1C6C(Coord *);
extern s32 func_800B5BDC(Coord *);
extern s32 func_800A23E8(Coord *, Coord *);

static inline void copy_coord(Coord *dst, Coord *src) {
    dst->x = src->x;
    dst->y = src->y;
}

static inline void pick_coord(Object *self, Bounds *bounds, Coord *out) {
    Coord pick;
    if (self->flags & 8) {
        func_800A33DC(&pick, bounds);
    } else {
        func_800B221C(&pick);
    }
    *out = pick;
}

static inline s32 coord_distance(Coord *from, Coord *to) {
    Coord target;
    copy_coord(&target, to);
    return func_800A23E8(from, &target);
}


static inline s32 in_room(void *room, Coord *coord) {
    return room == func_800B1F90(coord);
}

static inline s32 is_occupied(Coord *coord) {
    return func_800B5BDC(coord) != 0;
}

s32 func_800A5E24(Object *self, Coord *out, s32 sameRoom, s32 occupied) {
    Coord origin;
    Bounds bounds;
    Coord candidate;
    void *room;
    s32 distance;

    copy_coord(&origin, out);
    room = func_800B1F90(&origin);
    distance = 11;
    func_800B1F58(bounds.values);
    for (;;) {
        s32 tries;
        if (--distance < 5) {
            break;
        }
        tries = 20;
        for (;;) {
            s32 blocked;
            u8 refused;
            if (--tries == -1) {
                break;
            }
            pick_coord(self, &bounds, &candidate);
            refused = func_800A4314(self, &candidate) != 1;
            if (refused) continue;
            blocked = (self->flags & 8) && !(func_800B1C6C(&candidate) & 0x2000);
            if (blocked) continue;
            if (!sameRoom && in_room(room, &candidate)) continue;
            if (!occupied && is_occupied(&candidate)) continue;
            if (coord_distance(&candidate, &origin) >= distance) {
                *out = candidate;
                return 1;
            }
        }
    }
    return 0;
}
