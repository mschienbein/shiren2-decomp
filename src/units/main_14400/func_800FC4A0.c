#include "common.h"

typedef unsigned char u8;

typedef struct Unit Unit;

void *func_800A38FC(s32 size);
Unit *func_800FC4E0(Unit *obj, u8 kind);

/* Factory-table entry: construct in the supplied storage, or in a fresh 0xA0-byte block. */
Unit *func_800FC4A0(u8 kind, Unit *storage)
{
    if (storage != 0) {
        return func_800FC4E0(storage, kind);
    }
    return func_800FC4E0(func_800A38FC(0xA0), kind);
}
