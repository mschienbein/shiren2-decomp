#include "common.h"
typedef unsigned char u8;
typedef struct { s32 x, y; } Pos;
typedef struct { u8 v; } Dir;
extern void *func_800A27A4(void *out_direction, void *from, void *to);
extern void func_800A2758(Pos *p, Dir d);
static inline Pos *copy_position(Pos *to, Pos *from) {
    to->x = from->x;
    to->y = from->y;
    return to;
}
void func_800A2454(Pos *self, Pos *destination) {
    Pos copy;
    Dir direction;
    func_800A27A4(&direction, self, copy_position(&copy, destination));
    func_800A2758(self, direction);
}
