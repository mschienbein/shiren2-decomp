#include "common.h"

typedef unsigned char u8;

typedef struct {
    void *container;
    void *field4;
} Entry_800D0444;

typedef struct {
    u8 pad0[0x8];
    Entry_800D0444 *entries;
} Obj_800D0444;

void *func_800D0444(Obj_800D0444 *obj, u32 index) {
    return obj->entries[index].field4;
}
