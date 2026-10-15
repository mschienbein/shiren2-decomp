#include "map_rect.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

/* Geometry prefix of the supplied foreign unit; allocation/lifetime are external. */
typedef struct { MapPos position; u8 pad8[4]; MapRect bounds; } Source;


/* Struct-return forwarder: the hidden result pointer is passed through and returned. */
MapPos func_800A6B70(void *source, u8 mode, s32 arg) {
    Source *src = source;
    return func_800A694C(source, &src->bounds, mode, arg);
}
