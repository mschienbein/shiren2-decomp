#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;

typedef unsigned char u8;

typedef struct {
    s32 x;
    s32 y;
} Pos;

typedef struct {
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
} Rect;

typedef struct {
    u8 v;
} Dir;

typedef struct {
    u32 pad_hi : 7;
    u32 flag24 : 1;
    u32 pad_lo : 24;
} Flags800F1CBC;

typedef struct {
    Pos pos;
    u8 pad08[0x20 - 0x08];
    Flags800F1CBC flags20;
} Obj800F1CBC;

typedef struct Room Room;

extern s32 func_800A23E8(Pos *origin, Pos *vec);
extern u32 func_800B1C6C(Pos *pos);
extern s32 func_800A63FC(void *a, void *b);
extern void *func_800A2FD0(void *, void *, u8);
extern s32 func_800A24DC(void *self, Rect *r);
extern void *func_800A27A4(void *out_direction, void *from, void *to);
extern void func_800A2758(Pos *p, Dir d);
extern s32 func_800A251C(Pos *x, Pos *y);
extern void *func_800B1F90(void *pos);
extern s32 func_800B68F0(Room *, Pos *);




static inline s32 mode_is_one(void)
{
    return (D_80142F18.mode & 0xE0) == 32;
}

/* Accessor on a local copy of the flag word (method-style call: the copy is addressable). */
static inline s32 flags_test24(const Flags800F1CBC *flags)
{
    return flags->flag24;
}

s32 func_800F1CBC(Obj800F1CBC *self, Pos *target)
{
    Pos pos;
    Pos goal;
    Rect area;
    Pos *at;
    s32 dist;
    s32 steps;
    s32 open;
    Flags800F1CBC flags = self->flags20;

    if (flags_test24(&flags)) {
        return 1;
    }
    at = &pos;
    at->x = self->pos.x;
    at->y = self->pos.y;
    goal.x = target->x;
    goal.y = target->y;
    dist = func_800A23E8(at, &goal);
    if (dist < 2) {
        return 1;
    }
    open = mode_is_one() && !(func_800B1C6C(at) & 0x1000);
    if (open) {
        if (func_800B1C6C(at) & 0x2080) {
            func_800A2FD0(&area, at, 4);
            if (!func_800A24DC(target, &area)) {
                return 0;
            }
            steps = dist * 2;
            for (;;) {
                Dir dir;
                Pos *here;

                if (steps-- <= 0) {
                    return 0;
                }
                goal.x = target->x;
                goal.y = target->y;
                func_800A27A4(&dir, &pos, &goal);
                func_800A2758(&pos, dir);
                here = &pos;
                if (func_800B1C6C(here) & 0x4000) {
                    return 0;
                }
                if (func_800A251C(here, target)) {
                    return 1;
                }
            }
        } else {
            Pos *here = &pos;
            void *room;
            s32 inside;

            if (!(func_800B1C6C(here) & 0x800)) {
                return 0;
            }
            room = func_800B1F90(target);
            inside = 0;
            if (room != 0) {
                inside = func_800B68F0(room, here) != 0;
            }
            return inside;
        }
    }
    return func_800A63FC(self, target);
}
