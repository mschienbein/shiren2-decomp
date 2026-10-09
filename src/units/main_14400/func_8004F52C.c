#include "common.h"
typedef unsigned char u8;
typedef struct { s32 x, y; } Cell;
typedef struct { u32 from_x, from_y, to_x, to_y; } Path;
typedef struct { u8 category, kind, pad_02[10], appearance, pad_0D[3], variant; } Item;
typedef struct { u8 pad_00[0x3E]; signed char variant; } Object;
typedef struct { u8 pad_00[0x12]; unsigned short flags; } Effect;
extern s32 D_8013968C;
extern void func_80050CEC(s32, s32, Cell *, Cell *, float);
extern s32 func_8007920C(s32, s32, s32, s32);
extern Object *func_8007946C(s32, s32);
extern void func_80079560(s32, s32, s32);
extern Effect *func_80085294(void (*)(void *), u32, Path);
extern u8 func_800AC1AC(u8);
extern void func_800893D8(void *), func_800897AC(void *);
/* ODD_C: Keep the addressable level byte of the inlined item accessor;
 * this also preserves the switch's stack temporary and scheduling. */
static inline void set_level(Object *object, u8 variant) {
    u8 *level = &variant;
    s32 value;
    switch (*level) {
    case 2: value = 1; break;
    case 4: object->variant = 2; return;
    case 6: value = 3; break;
    /* ODD_C: level 0 is real; its explicit label shapes the compare tree
     * even though the default arm performs the same store. */
    case 0: object->variant = 0; return;
    default: object->variant = 0; return;
    }
    object->variant = value;
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
    case 0xE7:
        set_level(object, item->variant);
        break;
    }
}
void func_8004F52C(Item *item, Cell *from, Cell *to) {
    Path path;
    s32 slot;
    path.from_x = from->y; path.from_y = from->x;
    path.to_x = to->y; path.to_y = to->x;
    if ((u32)(D_8013968C - 0xB8) < 0x1A) {
        switch (D_8013968C) {
        case 0xB9: case 0xBA: case 0xD1: {
            s32 category = item->category;
            s32 kind = item->kind;
            s32 variation = 0;
            switch (category) {
            case 0xF:
                return;
            case 5: {
                switch (kind) {
                case 0x76: variation = 0; break;
                case 0x77: variation = 1; break;
                case 0x78: variation = 2; break;
                case 0x79: variation = 3; break;
                case 0x7A: variation = 4; break;
                }
                func_80050CEC(0x33, variation, from, to, 1.0f);
                return;
            }
            case 0x13:
                if (kind == 0xF2) {
                    kind = item->appearance;
                    category = func_800AC1AC(kind);
                }
                break;
            }
            slot = func_8007920C(category, kind, path.from_x, path.from_y);
            if (slot >= 0) {
                Object *object = func_8007946C(4, slot);
                Effect *effect;
                func_80079560(4, slot, 1);
                if (category == 0x10) set_variant(object, item, kind);
                effect = func_80085294(func_800893D8, slot, path);
                if (D_8013968C == 0xBA) effect->flags |= 0x1000;
                if (D_8013968C == 0xD1) effect->flags |= 0x2000;
            }
            break;
        }
        case 0xC0: case 0xC1: case 0xD0: {
            s32 value = item->category;
            s32 kind = item->kind;
            /* ODD_C: the redundant narrowing keeps the category a separate copy of
             * the loaded byte, as in the original (compare reads the load). */
            s32 category = (short)value;
            if (value == 0xF) break;
            if (kind == 0xF2) { kind = item->appearance; category = func_800AC1AC(kind); }
            slot = func_8007920C(category, kind, path.from_x, path.from_y);
            if (slot >= 0) {
                Object *object = func_8007946C(4, slot);
                Effect *effect;
                func_80079560(4, slot, 1);
                if (category == 0x10) set_variant(object, item, kind);
                effect = func_80085294(func_800897AC, slot, path);
                if (D_8013968C == 0xD0) effect->flags |= 0x1000;
                else if (D_8013968C == 0xC1) effect->flags |= 0x2000;
            }
            break;
        }
        case 0xB8: {
            slot = func_8007920C(0xB, 0xC8, path.from_x, path.from_y);
            func_80079560(4, slot, 1);
            func_80085294(func_800897AC, slot, path);
            break;
        }
        }
    }
}
