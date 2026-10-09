#include "common.h"
typedef struct { s32 x, y; } Position;
typedef struct Dir { unsigned char value; } Dir;
typedef struct { unsigned char pad0[9]; unsigned char kind9; } Object;
typedef struct { unsigned char field0; unsigned char kind1; } Cell;
extern Object *D_80147FE0;
extern s32 func_800A46BC(void *object, void *position, Dir *direction);
extern u32 func_800B1C6C(Position *position);
extern void *func_800A2594(Position *out, void *position, Dir direction);
extern void *func_800B4D80(Position *position);
extern s32 func_800A41EC(void *object, void *position);
s32 func_800D71F0(Position *position, Dir *direction) {
    Position adjacent;
    s32 blocked;
    Cell *cell;
    s32 status = func_800A46BC(D_80147FE0, position, direction);
    status ^= 1;
    if (status) return 0;
    blocked = 0;
    if (func_800B1C6C(position) & 0x80) blocked = (D_80147FE0->kind9 & 0xF) == 1;
    if (blocked) return 0;
    func_800A2594(&adjacent, position, *direction);
    cell = func_800B4D80(&adjacent);
    if (cell && cell->kind1 == 0xF4) return 0;
    func_800A2594(&adjacent, position, *direction);
    return func_800A41EC(D_80147FE0, &adjacent);
}
