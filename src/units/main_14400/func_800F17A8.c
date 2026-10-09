#include "common.h"
typedef unsigned char u8;
typedef struct { u8 field_00[9]; u8 field_09; } Object;
typedef struct Position Position;
typedef struct { u8 field_00[3]; u8 field_03; } Entry;
extern void *func_800B4D80(Position *position);
extern s32 func_800B502C(void *object);
extern s32 func_800F1848(Object *object, Entry *entry);
s32 func_800F17A8(Object *object, Position *position, s32 force) {
    Entry *entry = func_800B4D80(position);
    s32 valid = 0;
    if (entry && (entry->field_03 == (object->field_09 & 15) || force)) {
        if (!func_800B502C(position)) valid = 1;
    }
    if (valid) return func_800F1848(object, entry);
    return 0;
}
