#include "common.h"
#include "map_rect.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 pad0[0x1C]; u16 flags; } Object;
extern void *func_800B1F90(void *);
extern MapPos *func_800A33DC(MapPos *, MapRect *);
extern void *func_800B221C(MapPos *);
extern s32 func_800A4314(Object *, MapPos *);
extern u32 func_800B1C6C(MapPos *);
extern s32 func_800B5BDC(MapPos *);
extern s32 func_800A23E8(MapPos *, MapPos *);

static inline void copy_coord(MapPos *dst, MapPos *src) {
    dst->x = src->x;
    dst->y = src->y;
}

static inline void pick_coord(Object *self, MapRect *bounds, MapPos *out) {
    MapPos pick;
    if (self->flags & 8) {
        func_800A33DC(&pick, bounds);
    } else {
        func_800B221C(&pick);
    }
    *out = pick;
}

static inline s32 coord_distance(MapPos *from, MapPos *to) {
    MapPos target;
    copy_coord(&target, to);
    return func_800A23E8(from, &target);
}


static inline s32 in_room(void *room, MapPos *coord) {
    return room == func_800B1F90(coord);
}

static inline s32 is_occupied(MapPos *coord) {
    return func_800B5BDC(coord) != 0;
}

s32 func_800A5E24(Object *self, MapPos *out, s32 sameRoom, s32 occupied) {
    MapPos origin;
    MapRect bounds;
    MapPos candidate;
    void *room;
    s32 distance;

    copy_coord(&origin, out);
    room = func_800B1F90(&origin);
    distance = 11;
    func_800B1F58(&bounds);
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
