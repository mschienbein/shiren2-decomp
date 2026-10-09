#include "common.h"
/* Partial object view: only the two state words at +0x94/+0x98 are read here. */
typedef struct { unsigned char pad_00[0x94]; s32 field_94; s32 field_98; } Object800F6644;
extern s32 func_800E20CC(void *);
extern s32 func_800F4498(void *);
s32 func_800F6644(Object800F6644 *object) {
    if (object->field_94 || object->field_98 || func_800E20CC(object))
        return func_800F4498(object);
    return 0;
}
