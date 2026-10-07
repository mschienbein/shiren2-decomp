#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct { s32 x0; s32 x4; } Pos;
typedef struct {
    s32 x0;
    s32 x4;
    u8 pad8[0x4C];
    u8 x54;
    u8 pad55[3];
    void *x58;
    u8 pad5C[0x16];
    u8 x72;
    u8 pad73[0x19];
    u8 x8C[0x18];
    s32 xA4;
    s32 xA8;
} Unit;
extern u8 D_80142F1B;
extern u16 D_8014767C;
extern void *D_801476B8;
s32 func_800E8694(Unit *unit);
s32 func_800E8350(Unit *unit);
void *func_800A492C(Unit *unit, s32 a, s32 b, s32 c);
s32 func_800E7104(Unit *unit);
u8 func_800A6420(Unit *unit, void *target);
s32 func_800E7424(Unit *unit, Pos *pos);
s32 func_800CD278(void *list);
void *func_800B4D80(Pos *pos);
s32 func_800F6F94(Unit *unit, void *item);
void func_800AD868(Pos *pos);
s32 func_800CD538(void *list, void *item);
void *func_800AC244(u8 id);
s32 func_800AD8AC(void *item, Pos *pos);
s32 func_800F70B4(Unit *unit, Pos *pos);
s32 func_800E65C0(Unit *unit, Pos *pos, s32 arg);
s32 func_800E66EC(Unit *unit);
s32 func_800F7474(Unit *unit) {
    void *list;
    Pos pos;
    Pos *posPtr;
    void *item;
    if (((D_80142F1B >> 2) & 1) || (unit->x72 & 1)) {
        return 0;
    }
    if (D_8014767C & 0xC) {
        unit->xA4 = 1;
    }
    if (func_800E8694(unit)) {
        return func_800E8350(unit);
    }
    if (unit->xA4) {
        unit->x58 = func_800A492C(unit, 2, 1, 1);
        return func_800E7104(unit);
    }
    unit->x58 = 0;
    if (unit->xA8) {
        switch (func_800A6420(unit, D_801476B8)) {
            case 0:
                unit->x58 = D_801476B8;
                unit->x54 |= 4;
                return 0;
            case 1:
                return func_800E7424(unit, D_801476B8);
        }
    }
    list = unit->x8C;
    if (func_800CD278(list) > 0) {
        posPtr = &pos;
        pos.x0 = unit->x0;
        posPtr->x4 = unit->x4;
        item = func_800B4D80(posPtr);
        if (item != 0 && func_800F6F94(unit, item)) {
            func_800AD868(posPtr);
            func_800CD538(list, item);
            item = func_800AC244(0xCB);
            if (item != 0) {
                func_800AD8AC(item, posPtr);
            }
            return 1;
        }
        if (func_800F70B4(unit, &pos) && func_800E65C0(unit, &pos, 0)) {
            return 1;
        }
    }
    return func_800E66EC(unit);
}
