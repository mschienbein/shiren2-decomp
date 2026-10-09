#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
typedef struct { s32 x, y; } Vec2;
typedef struct { s32 x, y, z; } Pos3;
typedef struct Actor Actor;
struct Actor { void (*update)(Actor *); u8 pad[0xE]; u16 flags; u8 pad2[0x20]; float scale; u8 pad3[0x24]; s32 x5C, x60, x64, x68, x6C; };
extern void *func_800A27A4(void *, void *, void *);
extern void *func_80085938(s32, s32, Pos3, s32, s32, s32, s32);
extern s32 func_800A99D0(void);
extern void func_800861C0(Actor *);
static inline s32 hidden_mode(void) { return (D_80142F18.flags >> 2) & 1; }
static inline Vec2 *copy_cell(Vec2 *out, Vec2 *in) { out->x = in->x; out->y = in->y; return out; }
static inline void cell_position(Pos3 *out, Vec2 *in) { out->x = (in->y << 5) + 16; out->y = (in->x << 5) + 16; out->z = 0; }
void func_80050BAC(s32 owner, u16 flags, Vec2 *cell, Vec2 *sub, float scale) {
    Vec2 tmp;
    Pos3 pos;
    u8 direction;
    Actor *act;
    s32 visible, facing;
    func_800A27A4(&direction, cell, copy_cell(&tmp, sub));
    facing = direction;
    cell_position(&pos, cell);
    act = func_80085938(owner, -1, pos, 0, facing, 0, 0);
    act->flags |= flags | 4;
    visible = 0;
    if (func_800A99D0()) visible = hidden_mode() == 0;
    if (!visible) act->flags |= 0x100;
    act->x5C = cell->y;
    act->x68 = cell->x;
    act->x60 = sub->y;
    act->x6C = sub->x;
    act->update = func_800861C0;
    act->scale = scale;
}
