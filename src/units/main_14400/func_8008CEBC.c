#include "common.h"
typedef struct { unsigned char field00; unsigned char references; } Resource;
typedef struct { unsigned char pad00[0x74]; Resource *resource; } Slot;
/* Original a1 is dereferenced and stored as the resource pointer. */
s32 func_8008CEBC(Slot *slot, Resource *resource) {
    slot->resource = resource;
    resource->references++;
    return 0;
}
