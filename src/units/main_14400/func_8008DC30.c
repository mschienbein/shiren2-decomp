#include "common.h"

typedef unsigned char u8;
typedef float f32;

typedef struct Reader8008DC30 Reader8008DC30;

typedef struct {
    f32 x;
    f32 y;
    s32 count;
    void *entries;
} Record8008DC30;

typedef struct {
    u8 pad0[0xC];
    Record8008DC30 *record;
} State8008DC30;

extern void *func_80091450(u32 size);
extern u8 *func_8006A810(u8 *dst, s32 value, s32 count);
extern f32 func_8008E178(Reader8008DC30 *r);
extern s32 func_8008DF04(Reader8008DC30 *r);
extern s32 func_8008DB48(Reader8008DC30 *r, Record8008DC30 *record);
extern void func_80091544(void *item);

/* Chunk handler: read a 0x10-byte record and its entries; the chunk size must match. */
s32 func_8008DC30(Reader8008DC30 *reader, s32 expected, State8008DC30 *state) {
    s32 result = 0;
    State8008DC30 *out = state;
    s32 size;
    s32 extra;
    Record8008DC30 *record;

    /* ODD_C: single-pass error block grouping record allocation, entry parsing and size
     * validation before the shared cleanup; it also shapes the prologue scheduling. */
    do {
        record = func_80091450(0x10);
        if (record == 0) {
            result = -1;
            break;
        }
        func_8006A810((u8 *)record, 0, 0x10);
        record->x = func_8008E178(reader);
        record->y = func_8008E178(reader);
        record->count = func_8008DF04(reader);
        size = 0xC;
        if (record->count != 0) {
            extra = func_8008DB48(reader, record);
            if (extra < 0) {
                result = -1;
                break;
            }
            size = extra + 0xC;
        }
        if (size != expected) {
            result = -1;
            break;
        }
        out->record = record;
    } while (0);
    if (result != 0 && record != 0) {
        func_80091544(record);
    }
    return result;
}
