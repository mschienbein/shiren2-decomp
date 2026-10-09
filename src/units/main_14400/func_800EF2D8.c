#include "common.h"
typedef struct { unsigned char fields00[0xC8]; short fieldC8; s32 (*fieldCC)(void *); } Methods;
typedef struct { unsigned char fields00[0x24]; Methods *field24; } Object;
extern s32 func_800E20CC(Object *object);
/* Actor slot +0xD4 supplies the optional target pointer; this default ignores
 * it, while func_80109610 and func_80109DE4 share the same two-pointer slot. */
s32 func_800EF2D8(Object *object, void *target) {
    if (func_800E20CC(object)) {
        return object->field24->fieldCC((unsigned char *)object + object->field24->fieldC8);
    }
    return 0;
}
