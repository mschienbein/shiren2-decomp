#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    s32 x;
    s32 y;
} Point;

extern u16 D_80143450[54][76];

s32 func_800B1DF8(Point *pos) {
    s32 outside = 0;

    if (pos->y >= 76 || pos->x >= 54 || pos->y < 0 || pos->x < 0) {
        outside = 1;
    }
    if (outside) {
        return 0;
    }
    return (D_80143450[pos->x][pos->y] & 0xC120) == 0xC120;
}
