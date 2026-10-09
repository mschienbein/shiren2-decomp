#include "common.h"

/* The backing array consists of eight-byte collection/item references. */
typedef struct { void *container; void *item; } Pair;
typedef struct {
    unsigned char pad_00[8];
    Pair *field_08;
} AddressView;

Pair *func_800D0620(AddressView *object, s32 index) {
    return object->field_08 + index;
}
