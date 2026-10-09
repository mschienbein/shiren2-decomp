#include "common.h"

typedef unsigned char u8;

/* Predicate callback for func_800A0E1C: entry kind byte equals 4. */
typedef struct {
    u8 kind;
} Entry8009A2C8;

s32 func_8009A2C8(void *arg) {
    Entry8009A2C8 *entry = arg;

    return entry->kind == 4;
}
