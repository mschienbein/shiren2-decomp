#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
struct Item { u8 kind00, type01, flags02, mode03, count04; s8 direction05; };
struct ListSlot { s16 delta, index; void *(*call)(void *); };
struct UnitTable { u8 pad00[0x98]; ListSlot items; };
struct Unit { u8 pad00[0xA]; u8 floor0A; u8 pad0B[0x14]; u8 floor1F; u8 pad20[4]; UnitTable *table24; u8 pad28[0xDC]; s32 flag104; };
struct Object { u8 pad00[0x61]; u8 enabled61; u8 pad62[0x22]; s32 history84; Item *item88; };
/* The one-byte direction is passed left-justified when used by value. */
struct Direction { s8 value; Direction(s8 v) : value(v) {} };
extern "C" {
extern Unit *D_801476B8;
struct SelectionRecord { u8 kind, variant, field02, flags, row, field05, field06, field07, mode; s8 coordinates[2], status; };
extern SelectionRecord D_80142F18;
struct SelectionSave { u8 index, count, previous, field_03, masks[2], field_06, previous_count, field_08; s8 result; u8 field_0A; };
extern SelectionSave D_80142F24;
extern s16 D_801401F0[];
extern const u8 D_80138FA0[], D_80138FB0[], D_801521D0[], D_80151E38[];
void *func_800953C0(void *);
void func_80097240(void *, void *, const void *, const void *);
s32 func_800957C0(void *, void *, s32, void *, s32);
s32 func_800E2044(Unit *);
Item *func_800A6DC8(Unit *);
void *func_80093398(Object *);
s32 func_800CD090(void *, void *);
s32 func_80048440(void);
void func_80049BF0(s32);
s32 func_80049CB4(s32, ...);
void func_80094DA0(Object *);
s32 func_80046190(void *unused_receiver);
void *func_800C9E10(void);
void *func_800C9E00(void);
void func_800452C0(void *);
void func_800D10C4(void *);
void *func_800A6D40(Unit *);
s32 func_800EC6A4(Unit *, void *);
void func_801E8A84(s32);
void *func_800D8FB0(u32);
void *func_800DFC80(void *);
void *func_800D92C0(void *);
void *func_800D90C0(void *);
void *func_800DA010(void *);
void *func_800DF9A0(void *);
void *func_800DA620(void *);
void *func_800DF8C0(void *);
void *func_800DEF90(void *, u8 *, u8);
void *func_800DA530(void *, Direction);
void *func_80092E98(Object *);
}
struct Dialog {
    u8 pad00[0x4C];
    const void *table4C;
    void *entries50;
    s32 count54, selection58;
    Dialog() { func_800953C0(this); table4C = D_801521D0; }
    ~Dialog() { table4C = D_80151E38; }
};
/* ODD_C: the facing test is a store-flag helper; GCC then branches on ~direction (nor) as ROM does. */
static inline s32 has_direction(Item *item) { return item->direction05 != -1; }
/* The direction arrives as an int and is narrowed into the one-byte record. */
static inline void *step(s32 direction, u8 flags) {
    void *storage = func_800D8FB0(12);
    Direction value(direction);
    return func_800DEF90(storage, (u8 *)&value, flags);
}
static inline void *turn(s8 direction) {
    void *storage = func_800D8FB0(12);
    Direction value(direction);
    return func_800DA530(storage, value);
}
extern "C" void *func_80092640(Object *self) {
    Unit *unit = D_801476B8;
    s32 choices[8];
    if (self->enabled61 && D_80142F24.index != 9) {
        s32 allowed = 0;
        if (func_800E2044(unit)) allowed = unit->flag104 == 0;
        if (allowed) {
            Item *item = func_800A6DC8(unit);
            if (item && item->mode03 == 2) {
                if (item->kind00 == 15) {
                    s32 mode = D_80142F18.flags & 3;
                    if (mode == 1) {
                        D_801401F0[0] = 0x433;
                    } else if (mode == 2) {
                        D_801401F0[0] = 0x434;
                    } else {
                        D_801401F0[0] = 0x435;
                    }
                    Dialog dialog;
                    func_80097240(&dialog, D_801401F0, D_80138FA0, D_80138FB0);
                    dialog.selection58 = 0;
                    if (func_800957C0(&dialog, choices, 1, 0, 0) && choices[0] == 0x30)
                        return func_800DFC80(func_800D8FB0(8));
                } else {
                    s32 usable = unit->floor0A == unit->floor1F && has_direction(item) && !(item->flags02 & 0x20);
                    if (usable) {
                        void *command = func_80093398(self);
                        if (command) return command;
                    }
                }
            }
        }
    }
    for (;;) {
        s32 active = 0;
        s32 key;
        if (self->history84 > 0) {
            Unit *owner = D_801476B8;
            ListSlot *slot = &owner->table24->items;
            if (func_800CD090(slot->call((u8 *)owner + slot->delta), self->item88) >= 0)
                active = self->item88->kind00 == 9;
        }
        if (active) {
            /* ODD_C: both arms assign key 8 (separate assignments; this also lets
               the delay slots carry the store on each path as in ROM). */
            if (func_80048440()) {
                func_80049BF0(0);
                func_80049CB4(2);
                key = 8;
            } else {
                key = 8;
            }
        } else {
            func_80094DA0(self);
            key = func_80046190(self);
        }
        func_800452C0(func_800C9E10());
        switch (key) {
        case 6:
            if (!D_801476B8->flag104) {
                void *item = func_800A6D40(D_801476B8);
                if (item && func_800EC6A4(D_801476B8, item)) {
                    func_801E8A84(0x1B); func_801E8A84(0x17);
                    return func_800D92C0(func_800D8FB0(8));
                }
            }
            return func_800D90C0(func_800D8FB0(8));
        case 9:
            if (D_801476B8->flag104) return func_800DA010(func_800D8FB0(8));
            return func_800DF9A0(func_800D8FB0(8));
        case 21: return step(4, 0);
        case 20: return step(6, 0);
        case 19: return step(2, 0);
        case 22: return step(0, 0);
        case 23: return step(3, 0);
        case 24: return step(1, 0);
        case 25: return step(5, 0);
        case 26: return step(7, 0);
        case 29: return step(4, 2);
        case 28: return step(6, 2);
        case 27: return step(2, 2);
        case 30: return step(0, 2);
        case 31: return step(3, 2);
        case 32: return step(1, 2);
        case 33: return step(5, 2);
        case 34: return step(7, 2);
        case 37: return step(4, 4);
        case 36: return step(6, 4);
        case 35: return step(2, 4);
        case 38: return step(0, 4);
        case 39: return step(3, 4);
        case 40: return step(1, 4);
        case 41: return step(5, 4);
        case 42: return step(7, 4);
        case 45: return step(4, 6);
        case 44: return step(6, 6);
        case 43: return step(2, 6);
        case 46: return step(0, 6);
        case 47: return step(3, 6);
        case 48: return step(1, 6);
        case 49: return step(5, 6);
        case 50: return step(7, 6);
        case 13: return turn(4);
        case 12: return turn(6);
        case 11: return turn(2);
        case 14: return turn(0);
        case 15: return turn(3);
        case 16: return turn(1);
        case 17: return turn(5);
        case 18: return turn(7);
        case 10: return func_800DA620(func_800D8FB0(8));
        case 7: return func_800DF8C0(func_800D8FB0(8));
        case 8:
            if (D_80142F24.index != 9) {
                func_800D10C4(func_800C9E00());
                void *command = func_80092E98(self);
                if (command) return command;
            }
            break;
        }
    }
}
