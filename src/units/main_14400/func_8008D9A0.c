#include "common.h"

/* 0x14-byte group record: func_8008D7E0 allocates the 8-byte pair array at +0x08
 * (count at +0x04) and func_8008D8C0 the one at +0x10 (count at +0x0C);
 * func_8008D6AC frees both arrays. */
typedef struct {
    s32 field_00;
    s32 field_04;
    void *field_08;
    s32 field_0C;
    void *field_10;
} Entry;
typedef struct {
    unsigned char count;
    char reserved_01[3];
    Entry *entries;
} Collection;
typedef struct {
    char reserved_00[0x10];
    Collection field_10;
} State;
extern s32 func_8008DF04(void *);
extern void *func_80091450(u32);
extern unsigned char *func_8006A810(void *, s32, s32);
extern s32 func_8008D7E0(void *, Entry *);
extern s32 func_8008D8C0(void *, Entry *);

s32 func_8008D9A0(void *stream, s32 expected, State *state) {
    s32 status = 0;
    Collection *collection = &state->field_10;
    s32 consumed;
    s32 size;
    s32 i;
    state->field_10.count = func_8008DF04(stream);
    consumed = 4;
    size = state->field_10.count * sizeof(Entry);
    collection->entries = func_80091450(size);
    if (collection->entries == 0) {
        status = -1;
    } else {
        func_8006A810(collection->entries, 0, size);
        for (i = 0; i < collection->count; i++) {
            s32 amount;
            collection->entries[i].field_00 = func_8008DF04(stream);
            consumed += 4;
            collection->entries[i].field_04 = func_8008DF04(stream);
            consumed += 4;
            amount = func_8008D7E0(stream, &collection->entries[i]);
            if (amount < 0) {
                status = -1;
                break;
            }
            consumed += amount;
            collection->entries[i].field_0C = func_8008DF04(stream);
            consumed += 4;
            amount = func_8008D8C0(stream, &collection->entries[i]);
            if (amount < 0) {
                status = -1;
                break;
            }
            consumed += amount;
        }
        if (status == 0 && consumed != expected) {
            status = -1;
        }
    }
    return status;
}
