#include "common.h"
#include "item_records.h"

typedef unsigned char u8;
typedef short s16;

typedef struct {
    s32 x;
    s32 y;
} Pos;

typedef struct {
    u8 v;
} Dir;

typedef struct {
    s32 kind;
    u8 pad04[0xC];
    Pos pos10;
    s32 dir18;
} Msg;


extern s32 func_800AF28C(Item *, Msg *);
extern void func_800A2758(Pos *p, Dir d);
extern void *func_800A2594(Pos *out, void *arg, Dir cell);
extern u32 func_800B1C6C(Pos *pos);
extern void func_80127DA0(void *self, Pos *pos);
extern s32 D_801487C0;

/* Direction constructor: keeps the low three bits (eight compass directions). */
static inline void dir_init(Dir *d, s32 value)
{
    d->v = value & 7;
}

s32 func_801280F0(Item *self, Msg *msg)
{
    Pos pos;
    Pos *at;
    Dir dir;
    s32 i;

    if (msg->kind == 0x1B) {
        at = &pos;
        at->x = msg->pos10.x;
        at->y = msg->pos10.y;
        if (msg->dir18 != -1) {
            dir_init(&dir, msg->dir18);
            func_800A2758(at, dir);
        } else {
            for (i = 0;; i++) {
                Pos probe;
                Pos step;
                Dir cell;

                if (i >= 9) {
                    break;
                }
                D_801487C0 = (D_801487C0 + 1) % 9;
                if (D_801487C0 < 8) {
                    dir_init(&cell, D_801487C0);
                    func_800A2594(&step, &pos, cell);
                    probe = step;
                } else {
                    probe = pos;
                }
                if (!(func_800B1C6C(&probe) & 0x4000)) {
                    break;
                }
            }
        }
        func_80127DA0(self, &pos);
        if (self != 0) {
            self->field08->method0C((u8 *)self + self->field08->delta08, 3);
        }
        return 1;
    }
    return func_800AF28C(self, msg);
}
