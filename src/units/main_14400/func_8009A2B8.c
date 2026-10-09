#include "common.h"

typedef unsigned char u8;
typedef struct { u8 field_00; } Object;

/* Predicate callback invoked as s32 (*)(void *) (e.g. by func_800A0E1C). */
s32 func_8009A2B8(void *object) {
    Object *entry = object;
    return entry->field_00 == 3;
}
