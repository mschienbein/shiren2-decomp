#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct {
    s32 x;
    s32 y;
} Pair;

typedef struct {
    Pair pos;
    Pair size;
} Rect;

/* Three consecutive 16-byte rectangles; D_801429B8 and D_801429D8 are their +8 members. */
extern Rect D_801429B0;
extern Rect D_801429C0;
extern Rect D_801429D0;

static inline void pair_copy(Pair *dst, Pair *src) {
    *dst = *src;
}
void func_800A352C(void) {
    Rect rect;
    Pair *size;

    rect.pos.x = 0;
    rect.pos.y = 0;
    rect.size.x = 0;
    rect.size.y = 0;
    pair_copy(&D_801429B0.pos, &rect.pos);
    pair_copy(&D_801429B0.size, &rect.size);
    size = &rect.size;
    rect.pos.x = 10;
    rect.pos.y = 10;
    size->x = 0x2B;
    size->y = 0x41;
    pair_copy(&D_801429C0.pos, &rect.pos);
    pair_copy(&D_801429C0.size, &rect.size);
    size->x = 0x35;
    rect.pos.x = 0;
    rect.pos.y = 0;
    size->y = 0x4B;
    pair_copy(&D_801429D0.pos, &rect.pos);
    pair_copy(&D_801429D0.size, &rect.size);
}
