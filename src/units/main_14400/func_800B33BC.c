#include "common.h"
typedef unsigned char u8;
typedef struct { s32 x, y; } Pos;
typedef struct { Pos first, last; } Rect;
typedef struct { Pos current, first, last; } Iterator;
typedef struct { u8 kind, id, flags_02, pad_03[2]; signed char owner_05; u8 pad_06[6]; u8 flags_0C; } Item;
extern u8 D_80147620[];
extern Pos *func_800A3610(Pos *, Iterator *);
extern void *func_800B4D80(Pos *);
extern s32 func_800B502C(void *);
extern u32 func_800B1C6C(Pos *);
extern u8 func_800C57CC(void *, s32);
/* ODD_C: Compare the direct current position with the iterator's retained bound. */
static inline s32 iterator_active(s32 current, Iterator *iterator) { return current <= iterator->last.x; }
/* ODD_C: A narrow predicate retains the explicit masked zero comparison. */
static inline u8 zero_bits(u32 value) { return value == 0; }
static inline s32 available(Item *item, Pos *pos) {
    s32 valid = 0;
    if (!(item->flags_02 & 0x10) && !(func_800B1C6C(pos) & 0x2000)) valid = zero_bits(item->flags_02 & 0x20);
    return valid;
}
/* Pick a random eligible entity position inside the rectangle (zero position if none).
 * The original returns the caller's output pointer in v0 (see the external declaration fix). */
Pos *func_800B33BC(Pos *out, Rect *rect, u8 kind, s32 unrestricted) {
    Iterator iterator;
    Pos position;
    s32 count = 0;
    position.x = rect->first.x;
    position.y = rect->first.y;
    iterator.first = position;
    iterator.current = iterator.first;
    position.x = rect->last.x;
    position.y = rect->last.y;
    iterator.last = position;
    while (iterator_active(iterator.current.x, &iterator)) {
        Item *item;
        s32 valid;
        u8 item_kind;
        func_800A3610(&position, &iterator);
        item = func_800B4D80(&position);
        valid = 0;
        if (item && ~item->owner_05 == 0) valid = func_800B502C(&position) == 0;
        if (!valid) continue;
        item_kind = item->kind;
        if (item_kind == 16 && ((item->flags_0C >> 1) & 1)) continue;
        if (item_kind != kind && (kind != 0 || item_kind == 15 || item_kind == 16 || item_kind == 19)) continue;
        if (unrestricted || available(item, &position)) count++;
    }
    if (count) {
        count = (u8)func_800C57CC(D_80147620, (u8)(count - 1));
        iterator.current = iterator.first;
        for (;;) {
            Item *item;
            s32 valid;
            u8 item_kind;
            if (!iterator_active(iterator.current.x, &iterator)) {
                /* Ran out of cells: no position. */
                out->x = 0;
                out->y = 0;
                break;
            }
            func_800A3610(&position, &iterator);
            item = func_800B4D80(&position);
            valid = 0;
            if (item && ~item->owner_05 == 0) valid = func_800B502C(&position) == 0;
            if (!valid) continue;
            item_kind = item->kind;
            if (item_kind == 16 && ((item->flags_0C >> 1) & 1)) continue;
            if (item_kind != kind && (kind != 0 || item_kind == 15 || item_kind == 16 || item_kind == 19)) continue;
            if (unrestricted || available(item, &position)) {
                if (--count == -1) {
                    out->x = position.x;
                    out->y = position.y;
                    break;
                }
            }
        }
    } else {
        out->x = 0;
        out->y = 0;
    }
    return out;
}
