#include "common.h"

typedef unsigned char u8;

/* Partial view of a four-byte lookup entry; only byte +2 is read here. */
typedef struct {
    u8 pad0[0x2];
    u8 field_2;
} Entry;

extern Entry *func_800B51D4(void *object);

s32 func_800B56F0(void *object)
{
    Entry *entry = func_800B51D4(object);

    if (entry != 0) {
        return entry->field_2 - 1 < 100U;
    }
    return 0;
}
