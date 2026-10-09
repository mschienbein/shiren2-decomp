#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    s32 x;
    s32 y;
} Point;

/* 16-byte area at D_801480A0, read as one object by func_800A33DC. */
typedef struct {
    Point min;
    Point max;
} Rect;

extern Rect D_801480A0;
extern u16 D_801480CC;
extern u8 D_801480CE;
extern u8 D_801480CF;

static inline void copyPoint(Point *dst, Point *src)
{
    *dst = *src;
}

void func_800D73D0(void)
{
    Point min;
    Point max;

    min.x = 11;
    min.y = 22;
    max.x = 16;
    max.y = 28;
    copyPoint(&D_801480A0.min, &min);
    copyPoint(&D_801480A0.max, &max);
    D_801480CF = 0;
    D_801480CE = 0;
    D_801480CC = 0;
}
