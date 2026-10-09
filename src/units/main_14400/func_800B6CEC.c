#include "common.h"

typedef struct {
    s32 x;
    s32 y;
} Pair;
typedef struct {
    char reserved_00[0xC];
    s32 count_0C;
    s32 index_10;
    s32 field_14;
} Iterator;
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;

extern s32 func_800B68B0(void *);
extern void *func_800A33DC(Pair *, void *);
extern void func_800C25F4(Iterator *, void *, s32, s32);
extern void *func_800C2758(Pair *, Iterator *);
extern u32 func_800B1C6C(Pair *);
extern s32 func_800A23E8(Pair *, Pair *);

static inline s32 valid_direction(s32 direction) {
    return direction < 4;
}

static inline s32 has_next(Iterator *iterator) {
    return iterator->index_10 < iterator->count_0C;
}

static inline Pair *copy_pair(Pair *destination, Pair *source) {
    destination->x = source->x;
    destination->y = source->y;
    return destination;
}

Pair *func_800B6CEC(Pair *output, void *state, Pair *first, Pair *second) {
    Pair best;
    Iterator iterator;
    Pair current;
    Pair copy;
    s32 direction;
    s32 minimum;
    s32 valid = func_800B68B0(state);
    minimum = 0x4C;
    if (valid == 0) {
        func_800A33DC(output, state);
    } else {
        for (direction = 0; valid_direction(direction); direction++) {
            iterator.index_10 = 0;
            iterator.count_0C = 0;
            func_800C25F4(&iterator, state, direction, (D_80142F18.mode & 0xE0) == 0x40);
            while (has_next(&iterator)) {
                func_800C2758(&current, &iterator);
                if (func_800B1C6C(&current) & 0x800) {
                    s32 distance;
                    s32 first_distance;
                    copy.x = first->x;
                    copy.y = first->y;
                    first_distance = func_800A23E8(&current, &copy);
                    copy.x = second->x;
                    copy.y = second->y;
                    distance = first_distance - func_800A23E8(&current, &copy);
                    if (distance < minimum) {
                        best = current;
                        minimum = distance;
                    }
                }
            }
        }
        copy_pair(output, &best);
    }
    return output;
}
