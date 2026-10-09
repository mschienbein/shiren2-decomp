#include "common.h"
typedef unsigned char u8;
typedef struct { s32 x, y; } Position;
typedef struct Dir { u8 value; } Dir;
typedef struct { Position position; Dir direction; u8 pad9[0x15]; u8 flags1E; } Object;
typedef struct { u8 pad0[0x18]; short adjustment18; short pad1A; s32 (*query1C)(void *self, s32 kind); } CellVTable;
typedef struct { u8 kind0, kind1; u8 pad2[6]; CellVTable *vtable8; } Cell;
typedef struct { Position min, max; } Bounds;
extern Object *D_80147FE0;
extern Position D_80148000; /* path start position */
u8 D_8014802C = 0;
Position D_80148030 = { 0, 0 };
u8 D_80148038 = 0;
u8 D_80148039 = 0;
u8 D_8014803C[0x40] = { 0 };
u8 D_8014807C = 0;
extern Bounds *D_80148080;
extern s32 func_800A251C(Position *a, Position *b);
extern s32 func_800D5968(Position *position);
extern s32 func_800D5730(Position *position, u8 *direction);
extern void *func_800A7DEC(void *object);
extern s32 func_800A4CC4(void *object, void *value, void *direction);
extern u32 func_800B1C6C(Position *position);
extern void *func_800A2594(Position *out, Position *from, Dir direction);
extern void func_800A2F80(Dir *direction, s32 turn);
extern s32 func_800FCF3C(Position *position, s32 mode);
extern s32 func_800B56F0(Position *position);
extern void *func_800B4D80(Position *position);
extern s32 func_800A6E50(void *object, void *cell);
extern s32 func_800A4EFC(void *object, void *value);
extern void func_800A665C(Object *object, u8 *direction);
static inline Position *copy_position(Position *out, Position *in) { out->x = in->x; out->y = in->y; return out; }
static inline s32 invert(s32 value) { return value ^ 1; }
static inline s32 isStraight(Dir direction) { return !(direction.value & 1); }
static inline Dir *queuedDirection(Dir *out) {
    out->value = D_8014803C[D_80148039] & 7;
    return out;
}
static inline s32 atSaved(Position *p) { return func_800A251C(&D_80148030, p); }
s32 func_800D6D78(void) {
    Position origin, forward, right, left, corner;
    Dir queued, direction;
    s32 available;
    s32 same;
    Object *object;
    copy_position(&origin, &D_80147FE0->position);
    if (invert(atSaved(&origin))) return 1;
    if ((D_8014807C & 2) && func_800A251C(&D_80148000, &D_80148030)) return 1;
    if (D_80148080) {
        if (D_80148039 >= D_80148038) return 1;
        if (D_80148039 && invert(func_800D5968(&origin))) return 1;
        object = D_80147FE0;
        direction = *queuedDirection(&queued);
        available = func_800A4CC4(object, func_800A7DEC(object), &direction);
    } else {
        direction = D_80147FE0->direction;
        if (func_800A251C(&D_80148000, &origin)) {
            available = 1;
        } else {
            Position *here = &origin;
            if ((func_800B1C6C(here) & 0x1000) || invert(func_800D5968(here))) {
                available = 0;
            } else {
                available = func_800D5730(here, &direction.value);
                func_800A2594(&forward, here, direction);
                if (!available && (func_800B1C6C(&forward) & 0xE100) && isStraight(direction)) {
                    Dir side;
                    s32 turn;
                    side.value = (direction.value + 2) & 7;
                    func_800A2594(&right, here, side);
                    side.value = (direction.value - 2) & 7;
                    func_800A2594(&left, here, side);
                    turn = !(func_800B1C6C(&right) & 0xE100) * 2;
                    if (!(func_800B1C6C(&left) & 0xE100)) turn -= 2;
                    if (turn) {
                        side.value = (direction.value + turn) & 7;
                        if (func_800B1C6C(&forward) & 0x2100) {
                            func_800A2594(&corner, &forward, side);
                            if (!(func_800B1C6C(&corner) & 0xE100)) {
                                func_800A2F80(&direction, turn == 2 ? 1 : -1);
                                available = 1;
                            }
                        }
                        if (!available) {
                            func_800A2F80(&direction, turn);
                            available = 1;
                        }
                    }
                }
            }
        }
    }
    if (!available) return 1;
    same = func_800A251C(&D_80148000, &origin);
    func_800A2594(&forward, &origin, direction);
    if (func_800FCF3C(&forward, 0)) return 0;
    if (!same) {
        Cell *cell;
        if (func_800B56F0(&forward)) return 1;
        cell = func_800B4D80(&forward);
        if (cell) {
            s32 interact = 0;
            if (cell->kind0 == 16) interact = func_800A6E50(D_80147FE0, cell) != 0;
            if (interact) {
                s32 blocked = 0;
                if (!((D_80147FE0->flags1E >> 2) & 1)) blocked = 1;
                else {
                    CellVTable *table = cell->vtable8;
                    if (table->query1C((u8 *)cell + table->adjustment18, 35)) blocked = 1;
                }
                if (blocked) return 1;
            } else if (cell->kind1 == 0xF4) return 1;
        }
    }
    if (!available || !func_800A4EFC(D_80147FE0, &direction)) return 1;
    if (D_80148080) {
        D_80148039++;
        if (D_80148039 >= D_80148038 && (D_8014807C & 1)) func_800A665C(D_80147FE0, &D_8014802C);
    }
    D_80148030 = D_80147FE0->position;
    D_8014807C |= 2;
    return 2;
}
