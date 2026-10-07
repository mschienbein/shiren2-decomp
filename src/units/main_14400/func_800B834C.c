#include "common.h"
void func_800B81BC(void *ctx, s32 x, s32 y, unsigned short color);
void func_800B834C(void *ctx, s32 r, unsigned short color) {
    s32 x = 0;
    s32 y = r - 1;
    s32 d = r;
    while (x <= y) {
        func_800B81BC(ctx, x, y, color);
        x++;
        d += 1 - (x << 1);
        if (d <= 0) {
            y--;
            d += (y << 1) - 1;
        }
    }
}
