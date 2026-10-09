#include "common.h"

/* Map position with a user-declared member-wise copy constructor (as in func_800A694C);
   a rectangle is two corners and is returned by value through the hidden result pointer. */
struct Pos {
    s32 x;
    s32 y;
    Pos(const Pos &other) : x(other.x), y(other.y) {}
};
struct Rect { Pos min; Pos max; };
/* 0x14-byte D_801431F0 record returned by func_800B1F90; it starts with its rectangle. */
struct Record { Rect area; unsigned char pad10[4]; };

extern "C" {
extern Rect D_801429C0;
extern unsigned char D_80143392;
u32 func_800B1C6C(void *pos);
Record *func_800B1F90(void *pos);
Rect func_800B2EDC(void *object);
void func_800A3448(s32 *area, s32 *bounds);
}

/* The object's rectangle clipped to the map bounds. */
extern "C" Rect func_800B3024(void *object) {
    Rect area = func_800B2EDC(object);
    func_800A3448((s32 *)&area, (s32 *)&D_801429C0);
    return area;
}

/* The rectangle for a position: the func_800B1F90 record when D_80143392 is clear and the
   position has flag 0x1000, otherwise func_800B3024's clipped rectangle.
   The copy out of the func_800B1F90 record is member-wise (interleaved lw/sw), and the other path
   forwards the hidden result pointer to func_800B3024 (a0 = result, a1 = pos). */
extern "C" Rect func_800B3080(void *pos) {
    s32 ok = 0;
    if (D_80143392 == 0) { s32 f = func_800B1C6C(pos) & 0x1000; ok = f != 0; }
    if (ok) return func_800B1F90(pos)->area;
    return func_800B3024(pos);
}
