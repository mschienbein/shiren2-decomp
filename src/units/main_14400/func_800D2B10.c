#include "common.h"
typedef unsigned char u8;
typedef struct { s32 x0, y0, x1, y1; } Rect;
typedef struct { s32 x, y; } Point;
typedef struct { unsigned char pad_00[4]; Rect *bounds_04; } Target;
typedef struct { Point position; unsigned char pad_08[0x78]; Target *owner_80; } Entry;
extern s32 func_800A9070(s32 *iterator, s32 kind);
extern void *func_800A910C(s32 *iterator);
extern s32 func_800A31C8(Rect *rect, Point *pos);
extern s32 func_800E0F40(void *obj);

static inline Entry *next_entry(s32 *iterator) {
    return func_800A910C(iterator);
}

/* Highest-valued kind-0x57 entry owned by target and inside its bounds. */
Entry *func_800D2B10(Target *target) {
    Entry *best = 0;
    u8 best_value = 0;
    s32 iterator = 0;
    while (func_800A9070(&iterator, 0x57)) {
        Entry *entry = next_entry(&iterator);
        s32 valid = 0;
        u8 value;
        if (entry->owner_80 == target) {
            valid = func_800A31C8(target->bounds_04, &entry->position) != 0;
        }
        if (!valid) continue;
        value = func_800E0F40(entry);
        if (best_value < value) {
            best = entry;
            best_value = value;
        }
    }
    return best;
}
