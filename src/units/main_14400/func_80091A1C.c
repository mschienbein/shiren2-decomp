#include "common.h"

typedef struct {
    s32 field_00;
    s32 next_04;
    s32 next_08;
    s32 next_0C;
    s32 next_10;
} Entry;

typedef struct {
    s32 count_00;
    Entry *entries_04;
} Collection;

typedef struct {
    s32 index_00;
    Collection *collection_04;
} Cursor;

extern void func_80045A24(s32 sound);

s32 func_80091A1C(Cursor *cursor, s32 direction) {
    Collection *collection = cursor->collection_04;
    s32 previous = cursor->index_00;
    s32 next;
    if (collection != 0 && previous >= 0 && previous < collection->count_00) {
        next = -1;
        switch (direction) {
        case 0: next = collection->entries_04[previous].next_04; break;
        case 1: next = collection->entries_04[previous].next_08; break;
        case 2: next = collection->entries_04[previous].next_0C; break;
        case 3: next = collection->entries_04[previous].next_10; break;
        }
        if (next >= 0) {
            Collection *current = cursor->collection_04;
            if (current != 0 && next < current->count_00) {
                cursor->index_00 = next;
            }
        }
    }
    if (cursor->index_00 != previous) {
        func_80045A24(2);
        return 1;
    }
    return 0;
}
