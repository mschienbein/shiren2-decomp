#include "common.h"
typedef unsigned char u8;
typedef struct { s32 x, y; } Cell;
typedef struct { u8 category, kind, pad_02[10], appearance, pad_0D[3], variant; } Item;
typedef struct { u8 pad_00[0x14]; float height; u8 pad_18[0x26]; signed char variant; } Object;
typedef struct { u8 pad_00[6]; unsigned short id; u8 pad_08[0xC]; s32 link; u8 pad_18[0x44]; s32 x; u8 pad_60[8]; s32 y; } Effect;
extern s32 D_8013968C;
extern void func_80050E44(s32, Cell *), func_80079560(s32, s32, s32), func_80084A20(void), func_80084B80(void);
extern s32 func_8007920C(s32, s32, s32, s32), func_80084C00(s32, void (*)(void *), s32, s32, s32);
extern Object *func_8007946C(s32, s32);
extern Effect *func_80085154(void (*)(void *), s32);
extern void *func_800851B0(s32);
extern u8 func_800AC1AC(u8);
extern void func_800871E8(void *), func_800872C4(void *), func_800881C0(void *), func_80088204(void *), func_80088240(void *), func_8008AEC8(void *);
/* ODD_C: the level helper takes the item level by address (like
 * flag_test(&flags) in func_800AF28C); the caller's addressable copy is the
 * original's level byte stack temporary, and the helper also shapes the switch
 * scheduling. */
static inline void set_level(Object *object, u8 *level) {
    switch (*level) {
    /* ODD_C: level 0 (an unlevelled item) shares the default store; the
     * explicit label keeps it in the original decision tree (slti/bnezl). */
    case 0:
    default: object->variant = 0; break;
    case 2: object->variant = 1; break;
    case 4: object->variant = 2; break;
    case 6: object->variant = 3; break;
    }
}
static inline void set_variant(Object *object, Item *item, s32 kind) {
    switch (kind) {
    case 0xD5: {
        /* ODD_C: the flag is computed before the test so it is live across the
         * branch (original sltu result register). */
        s32 lit = item->variant != 0;
        if (item->variant) object->variant = lit;
        break;
    }
    case 0xE7: {
        u8 level = item->variant;
        set_level(object, &level);
        break;
    }
    }
}
void func_8004F0D0(Item *item, Cell *cell) {
    Effect *effect;
    s32 slot;
    switch (D_8013968C) {
    case 0xC4: {
        s32 category, kind;
        func_800851B0(0x94);
        category = item->category;
        kind = item->kind;
        if (category != 0xF) {
            Object *object;
            slot = func_8007920C(category, kind, cell->y, cell->x);
            object = func_8007946C(4, slot);
            func_80079560(4, slot, 1);
            if (category == 0x10) set_variant(object, item, kind);
            func_80085154(func_80088240, slot);
        }
        return;
    }
    case 0xC8:
    case 0xCE: {
        s32 category = item->category;
        if (category != 0xF) {
            slot = func_8007920C(category, item->kind, cell->y, cell->x);
            if (slot >= 0) {
                func_80079560(4, slot, 1);
                effect = func_80085154(func_800871E8, slot);
                effect->x = cell->y;
                effect->y = cell->x;
            }
        }
        break;
    }
    case 0xC9:
        if (item->kind == 0xEA) {
            func_80084A20();
            func_800851B0(0x21);
            effect = func_80085154(func_8008AEC8, -1);
            effect->x = cell->y;
            effect->y = cell->x;
            func_80084B80();
        }
        func_80050E44(0xA6, cell);
        return;
    case 0xCB:
    case 0xCC: {
        s32 kind = item->kind;
        s32 category = item->category;
        s32 slot;
        Object *object;
        if (kind == 0xF2) {
            kind = item->appearance;
            category = func_800AC1AC(kind);
        }
        slot = func_8007920C(category, kind, cell->y, cell->x);
        object = func_8007946C(4, slot);
        func_80079560(4, slot, 1);
        if (D_8013968C == 0xCC) object->height = 7.0f;
        effect = func_80085154(func_800881C0, slot);
        if (category == 0x10) set_variant(object, item, kind);
        effect->x = cell->y;
        effect->y = cell->x;
        return;
    }
    case 0xCD: {
        effect = func_80085154(func_80088204, 0);
        effect->x = cell->y;
        effect->y = cell->x;
        effect->link = func_80084C00(effect->id, func_800881C0, cell->y, cell->x, 0);
        return;
    }
    case 0xCF: {
        Object *object;
        slot = func_8007920C(0xB, 0xC8, cell->y, cell->x);
        object = func_8007946C(4, slot);
        func_80079560(4, slot, 1);
        object->height += 200.0f;
        func_80085154(func_800872C4, slot);
        func_800851B0(0xB0);
        return;
    }
    case 0xD2: {
        Object *object;
        slot = func_8007920C(item->category, item->kind, cell->y, cell->x);
        object = func_8007946C(4, slot);
        func_80079560(4, slot, 1);
        object->height += 200.0f;
        func_80085154(func_800872C4, slot);
        return;
    }
    }
}
