#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { s32 x, y; } Vec;
typedef struct { Vec start; Vec pos; Vec end; } Iter;
typedef struct { u8 pad[0x3DC]; s32 x3DC; } Self;
typedef struct { s32 x0; s32 y0; s32 x1; s32 y1; } Rect;
extern Rect D_801429C0;
extern u8 D_801431F0[];
extern u8 D_8014344C;
extern u8 D_80143392;
void *func_800A3610(void *out, void *it);
void func_800B1BE0(Vec *v, s32 arg);
void func_800B1B58(Vec *v, u16 flags);
void func_800B6728(void *dst, void *r);
void func_800B17A4(void);

static inline void iter_set_start(Iter *it, Vec *p, s32 x, s32 y) {
    p->x = x;
    p->y = y;
    it->pos = *p;
    it->start = it->pos;
}
static inline void iter_set_end(Iter *it, Vec *p, s32 x, s32 y) {
    p->x = x;
    p->y = y;
    it->end = *p;
}
void func_800C0A20(Self *self) {
    Iter it;
    Vec tmp;
    Iter *iter = &it;
    Vec *cur = &tmp;
    iter_set_start(&it, &tmp, D_801429C0.x0, D_801429C0.y0);
    iter_set_end(&it, &tmp, D_801429C0.x1, D_801429C0.y1);
    for (;;) {
        s32 valid = !(it.start.x > iter->end.x);
        if (!valid) {
            break;
        }
        func_800A3610(cur, iter);
        func_800B1BE0(cur, 0x4000);
        func_800B1B58(cur, 0x1280);
    }

    func_800B6728(D_801431F0, &D_801429C0);
    self->x3DC = 1;
    D_8014344C = 1;
    func_800B17A4();
    D_80143392 = 5;
}
