#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    s32 x;
    s32 y;
} Pos;

/* One-byte direction (0..7) passed by value. */
typedef struct {
    u8 value;
} Dir;

typedef struct {
    Pos pos;
    Dir dir;
} Request;

/* 0x38-byte effect object built by func_80111E08 and torn down by func_800C2D0C. */
typedef struct {
    u8 data[0x38];
} Effect;

void *func_800B5A18(void *out, void *pos, Dir dir, s32 count, u16 mask);
void func_800A2758(Pos *p, Dir d);
s32 func_80111A20(void *self, void *req);
Effect *func_80111E08(Effect *o, void *a1, void *a2, Pos *pos, u8 *color);
void func_800C2D0C(Effect *effect);

static inline void pos_copy(Pos *dst, Pos *src)
{
    dst->x = src->x;
    dst->y = src->y;
}

static inline u8 dir_opposite(Dir *dir)
{
    return (dir->value + 4) & 7;
}

void func_8011F370(void *self, Request *req)
{
    Pos pos;
    Pos out;
    Pos in;
    Effect effect;
    Dir dir;
    u8 back;
    Dir facing = req->dir;

    dir = facing;
    pos_copy(&pos, &req->pos);
    pos_copy(&in, &pos);
    func_800B5A18(&out, &in, facing, 10, 0xC000);
    pos = out;
    func_800A2758(&pos, dir);
    if (func_80111A20(self, req)) {
        back = dir_opposite(&dir);
        func_80111E08(&effect, req, self, &pos, &back);
        func_800C2D0C(&effect);
    }
}
