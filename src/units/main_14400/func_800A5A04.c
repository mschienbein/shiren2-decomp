#include "map_rect.h"
typedef struct { unsigned char pad_00[0xC]; MapRect field_0C; unsigned char pad_1C[2]; unsigned char field_1E; } Object;
static inline s32 variant(Object *object) {
    return (object->field_1E >> 1) & 1;
}
void func_800A5A04(Object *object) {
    MapRect value;
    if (variant(object)) value = func_800B3080(object);
    else value = func_800B3024(object);
    object->field_0C = value;
}
