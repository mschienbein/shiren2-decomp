#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { s32 x; s32 y; } Point800B81BC;
u32 func_800B1C6C(Point800B81BC *pos);
void func_800B1AE0(Point800B81BC *pos, u16 kind);
static inline s32 beyond(s32 value, s32 limit) {
    return limit < value;
}

void func_800B81BC(Point800B81BC *center, s32 rx, s32 ry, u16 kind) {
    Point800B81BC pos;
    Point800B81BC tmp;
    s32 i;

    for (i = center->y - rx; !beyond(i, center->y + rx); i++) {
        tmp.x = center->x - ry;
        tmp.y = i;
        pos = tmp;
        if (func_800B1C6C(&pos) & 0x4000) {
            func_800B1AE0(&pos, kind);
        }
        tmp.x = center->x + ry;
        tmp.y = i;
        pos = tmp;
        if (func_800B1C6C(&pos) & 0x4000) {
            func_800B1AE0(&pos, kind);
        }
    }
    for (i = center->y - ry; !beyond(i, center->y + ry); i++) {
        tmp.x = center->x - rx;
        tmp.y = i;
        pos = tmp;
        if (func_800B1C6C(&pos) & 0x4000) {
            func_800B1AE0(&pos, kind);
        }
        tmp.x = center->x + rx;
        tmp.y = i;
        pos = tmp;
        if (func_800B1C6C(&pos) & 0x4000) {
            func_800B1AE0(&pos, kind);
        }
    }
}
