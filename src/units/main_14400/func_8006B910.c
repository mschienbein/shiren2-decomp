#include "common.h"
extern unsigned char D_8019F8C0[];
void func_8006B910(s32 left, s32 top, s32 right, s32 bottom) {
    s32 x;
    if (left < 0 || top < 0 || right < 0 || bottom < 0) {
        left = 0;
        top = 0;
        right = 36;
        bottom = 26;
    } else {
        if (right >= 36) right = 35;
        if (bottom >= 26) bottom = 25;
    }
    for (; top <= bottom; top++) {
        unsigned char *cursor = &D_8019F8C0[top * 36 + left];
        for (x = left; x <= right; x++) *cursor++ = 0;
    }
}
