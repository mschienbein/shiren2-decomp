#include "common.h"
#include "map_position_iterator.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { MapRect bounds; u8 pad10[4]; } Room;
typedef struct { u8 pad_00[0x3DC]; s32 count_3DC; u8 pad_3E0[0x578]; u16 flags_958; u8 pad_95A[2]; s32 active_95C[16]; MapRect selected_99C; } Object;
typedef struct { u8 pad_00[2]; u8 flags_02; u8 pad_03[9]; union { u32 amount; struct { u8 flags_0C; u8 rest[3]; } bytes; } field_0C; } Item;
typedef struct { u8 pad_00[8]; short adjustment_08, unused_0A; void (*destroy_0C)(void *, s32); } Table;
typedef struct { u8 pad_00[0x1C]; u16 flags_1C; u8 pad_1E[6]; Table *vtable_24; } Actor;
extern Room D_801431F0[];
extern u16 D_80156AD8;
extern s32 func_800B68B0(void *);
extern MapPos *func_800A33DC(MapPos *, MapRect *);
extern void *func_800AC5B4(s32, s32);
extern void *func_80124820(void *);
extern s32 func_800AC670(void *);
extern s32 func_800AE18C(void *, MapPos *);
extern s32 func_800B4F74(MapPos *);
extern void *func_800AC244(u8);
extern Actor *func_800AA63C(void);
extern void func_800A58FC(void *, MapPos *);
static inline void read_bounds(MapRect *out, Room *room) {
    out->min.x = room->bounds.min.x; out->min.y = room->bounds.min.y;
    out->max.x = room->bounds.max.x; out->max.y = room->bounds.max.y;
}
static inline void select_bounds(Object *self, Room *room) {
    MapRect copy;
    read_bounds(&copy, room);
    self->selected_99C.min = copy.min;
    self->selected_99C.max = copy.max;
}
static inline MapPos *copy_position(MapPos *out, MapPos *in) { out->x = in->x; out->y = in->y; return out; }
static inline s32 iterator_valid(MapPositionIterator *it) { return it->cur.x <= it->end.x; }
static inline void move_actor(Actor *actor, MapPos *position) { func_800A58FC(actor, position); }
static inline s32 needs_placement(Item *item) { return func_800AC670(item) ^ 1; }
s32 func_800BDD54(Object *self) {
    s32 found;
    s32 i;
    if (!(self->flags_958 & 1)) return 0;
    found = 0;
    i = 0;
    for (;;) {
        s32 more = i < self->count_3DC;
        if (!more) break;
        if (self->active_95C[i] && !func_800B68B0(&D_801431F0[i])) {
            self->active_95C[i] = 0;
            select_bounds(self, &D_801431F0[i]);
            found = 1;
            break;
        }
        i++;
    }
    if (!found) return 0;
    {
        MapPos position;
        MapPos temp;
        MapPositionIterator iterator;
        Item *item;
        Actor *actor;
        s32 amount;
        func_800A33DC(&position, &self->selected_99C);
        item = func_80124820(func_800AC5B4(0x10, 1));
        if (needs_placement(item)) {
            item->field_0C.bytes.flags_0C |= 2;
            item->flags_02 &= ~0x10;
            func_800AE18C(item, &position);
        }
        amount = D_80156AD8;
        iterator.start = *copy_position(&temp, &self->selected_99C.min);
        iterator.cur = iterator.start;
        iterator.end = *copy_position(&temp, &self->selected_99C.max);
        while (iterator_valid(&iterator)) {
            func_800A3610(&position, &iterator);
            if (!func_800B4F74(&position)) {
                Item *spawned = func_800AC244(0xCC);
                if (spawned) {
                    spawned->field_0C.amount = amount;
                    func_800AE18C(spawned, &position);
                }
            }
        }
        func_800A33DC(&temp, &self->selected_99C);
        position = temp;
        actor = func_800AA63C();
        if (actor) {
            if (actor->flags_1C & 8) {
                actor->vtable_24->destroy_0C((u8 *)actor + actor->vtable_24->adjustment_08, 3);
                return 1;
            }
            move_actor(actor, &position);
        }
    }
    return 1;
}
