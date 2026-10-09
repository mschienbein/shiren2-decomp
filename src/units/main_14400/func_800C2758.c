#include "common.h"
typedef unsigned char u8;
typedef struct { s32 first, second; } Pair;
typedef struct { u8 value; } Dir;
typedef struct { Pair field0; u8 field8; u8 pad9[7]; s32 field10; } Object;
extern void func_800A2758(Pair *position, Dir direction);
static inline void copy(Pair *dest, const Pair *source) {
    dest->first = source->first;
    dest->second = source->second;
}
Pair *func_800C2758(Pair *result, Object *obj) {
    Pair saved;
    Dir direction;
    copy(&saved, &obj->field0);
    direction.value = obj->field8;
    func_800A2758(&obj->field0, direction);
    obj->field10++;
    copy(result, &saved);
    return result;
}
