#include "common.h"
#include "map_line_iterator.h"

/* A complete 20-byte room: geometry plus four side counts, not a bare rectangle. */
typedef struct {
    MapRect geometry;
    unsigned char side_counts[4];
} RoomRecord;
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05; unsigned short field_06; unsigned char mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;

extern s32 func_800B68B0(RoomRecord *);
extern MapPos *func_800A33DC(MapPos *, MapRect *);
extern u32 func_800B1C6C(MapPos *);
extern s32 func_800A23E8(MapPos *, MapPos *);

static inline s32 valid_direction(s32 direction) {
    return direction < 4;
}

static inline s32 has_next(MapLineIterator *iterator) {
    return iterator->index < iterator->length;
}

static inline MapPos *copy_pair(MapPos *destination, MapPos *source) {
    destination->x = source->x;
    destination->y = source->y;
    return destination;
}

MapPos *func_800B6CEC(MapPos *output, RoomRecord *state, MapPos *first, MapPos *second) {
    MapPos best;
    MapLineIterator iterator;
    MapPos current;
    MapPos copy;
    s32 direction;
    s32 minimum;
    s32 valid = func_800B68B0(state);
    minimum = 0x4C;
    if (valid == 0) {
        func_800A33DC(output, &state->geometry);
    } else {
        for (direction = 0; valid_direction(direction); direction++) {
            iterator.index = 0;
            iterator.length = 0;
            func_800C25F4(&iterator, &state->geometry, direction, (D_80142F18.mode & 0xE0) == 0x40);
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
