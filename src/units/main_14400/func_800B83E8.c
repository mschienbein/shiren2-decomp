#include "common.h"
typedef struct { s32 x, y; } Position;
extern u32 func_800B1C6C(Position *position);
extern void func_800B480C(Position *position);
static inline Position *set_position(Position *out, s32 x, s32 y) { out->x=x; out->y=y; return out; }
static inline s32 after(s32 value, s32 limit) { return value > limit; }
void func_800B83E8(Position *center, s32 first, s32 second) {
    Position position, temporary;
    s32 i;
    for (i = center->y - first; ; i++) {
        if (after(i, center->y + first)) break;
        set_position(&temporary, center->x - second, i);
        position = temporary;
        if (func_800B1C6C(&position) & 0x4000) func_800B480C(&position);
        set_position(&temporary, center->x + second, i);
        position = temporary;
        if (func_800B1C6C(&position) & 0x4000) func_800B480C(&position);
    }
    for (i = center->y - second; ; i++) {
        if (after(i, center->y + second)) break;
        set_position(&temporary, center->x - first, i);
        position = temporary;
        if (func_800B1C6C(&position) & 0x4000) func_800B480C(&position);
        set_position(&temporary, center->x + first, i);
        position = temporary;
        if (func_800B1C6C(&position) & 0x4000) func_800B480C(&position);
    }
}
