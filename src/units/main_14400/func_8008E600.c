#include "common.h"

typedef unsigned short u16;

typedef struct {
    s32 offset;
    s32 size;
} Entry8008E600;

typedef struct {
    s32 base;
    s32 dataOffset;
    u32 count;
    Entry8008E600 *entries;
} Table8008E600;

typedef struct {
    char pad0[0x6];
    u16 index;
} Obj8008E600;

extern s32 func_8008EBA8(Obj8008E600 *obj, u32 addr, s32 size, u16 arg3);
s32 func_8008E600(Obj8008E600 *obj, Table8008E600 *table, u32 index, u16 arg3) {
    s32 result;
    /* ODD_C: single-pass block that skips opening an out-of-range archive entry; it also
     * shapes the argument register binding and the bnezl index store. */
    do {
        if (index >= table->count) {
            result = -1;
            break;
        }
        obj->index = index;
        result = func_8008EBA8(obj, table->base + table->dataOffset + table->entries[index].offset,
                               table->entries[index].size, arg3);
    } while (0);
    return result;
}
