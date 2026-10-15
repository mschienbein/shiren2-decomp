#include "common.h"
#include "map_rect.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 value; } Dir;
typedef struct { MapRect bounds; u8 side_counts[4]; } Room;
typedef struct { char pad[4]; Room *room; } Obj;
s32 func_800B68B0(Room *);
void *func_800B6A98(MapPos *out, Room *room, s32 index);
void *func_800A2594(MapPos *out, MapPos *from, Dir dir);
s32 func_800A31C8(MapRect *, MapPos *);
void *func_800B4D80(void *pos);
void func_800B1B58(MapPos *pos, u16 flags);
static inline void setDir(Dir *d, s32 v) { d->value = v & 7; }
void func_800D23AC(Obj *o) {
    MapPos base;
    MapPos pos;
    s32 i, j;
    s32 n = func_800B68B0(o->room);
    for (i = 0; ; i++) {
        Room *map;
        if (!(i < n)) break;
        map = o->room;
        func_800B6A98(&base, map, i);
        for (j = 0; ; j++) {
            Dir d;
            s32 ok;
            if (!(j < 8)) break;
            setDir(&d, j);
            func_800A2594(&pos, &base, d);
            ok = 0;
            if (func_800A31C8(&o->room->bounds, &pos)) ok = func_800B4D80(&pos) == 0;
            if (ok) func_800B1B58(&pos, 0x10);
        }
    }
}
