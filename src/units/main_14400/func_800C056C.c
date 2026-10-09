#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x, y; } Pair;
typedef struct { Pair min, max; } Rect;
typedef struct { Rect rect; u8 tag[4]; } Room;
typedef struct {
    u8 pad_000[0x3DC]; s32 count;
    u8 pad_3E0[0x24]; Room rooms[17];
    s32 edges[16][16];
} Layout;
extern u8 D_80147620[];
extern s32 func_800C5844(void *rng, u8 base, u8 top);
extern Rect *func_800A324C(Rect *out, Rect *a, Rect *b);
extern void func_800A3180(Rect *rect);
extern s32 func_800C00B4(u8 *self, void *rect);

/* ODD_C: field/predicate accessors preserve the original loop and copy scheduling. */
static inline s32 room_count(Layout *self) {
    return self->count;
}

static inline s32 edge_kind_valid(u16 kind) {
    return kind < 2;
}


static inline s32 pair_x(Pair *pair) {
    return pair->x;
}

static inline Rect *room_bounds(Layout *self, s32 index) {
    /* local-arithmetic-qualification: direct rooms[index] folds 0x404 into
       the index, and pointer arithmetic always places the object base first;
       the original adds the room offset to the object base, then adds the
       rooms[] offset as a separate step (its address chain is not
       sched-launched). The result denotes one complete room in rooms[17]. */
    s32 address = index * (s32)sizeof(Room) + (s32)self;
    address += 0x404;
    return (Rect *)address;
}



void func_800C056C(Layout *self) {
    u8 connected = 1;
    s32 tries, extra;
    self->edges[0][0] = 1;
    while (connected < room_count(self)) {
        s32 selected_a = 0;
        s32 selected_b = 1;
        u16 kind = 1;
        for (; edge_kind_valid(kind); kind--) {
            s32 best = 0xFFFF;
            s32 i, j;
            for (i = 0; ; i++) {
                if (i >= room_count(self) - 1) break;
                for (j = i + 1; j < self->count; j++) {
                    if (self->edges[i][i] != self->edges[j][j]
                        && self->edges[j][i] == kind
                        && self->edges[i][j] <= best) {
                        selected_a = i;
                        selected_b = j;
                        best = self->edges[i][j];
                    }
                }
            }
            if (best != 0xFFFF) break;
        }
        connected++;
        self->edges[selected_b][selected_a] = 2;
        self->edges[selected_b][selected_b] = 1;
        self->edges[selected_a][selected_a] = 1;
    }
    extra = 3;
    for (tries = 0; ; tries++) {
        Rect bounds, test;
        s32 a, b;
        if (tries >= 10) break;
        a = (u8)func_800C5844(D_80147620, 0, self->count - 2);
        b = (u8)func_800C5844(D_80147620, a + 1, self->count - 1);
        if (self->edges[b][a] != 1) continue;
        func_800A324C(&bounds, room_bounds(self, a), room_bounds(self, b));
        func_800A3180(&bounds);
        {
            Pair *min = &bounds.min;
            Pair *max = &bounds.max;
            test.min.x = bounds.min.x;
            test.min.y = min->y;
            test.max.x = pair_x(max);
            test.max.y = max->y;
        }
        if (func_800C00B4((u8 *)self, &test)) continue;
        self->edges[b][a] = 2;
        if (--extra <= 0) break;
    }
}
