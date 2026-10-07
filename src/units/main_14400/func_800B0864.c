#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;

typedef struct Entry800B0864 Entry800B0864;
typedef struct { void *field_0; u16 count_4; } Obj800B0864;
Entry800B0864 *func_800AFD78(void *value, u8 index);

Entry800B0864 *func_800B0864(Obj800B0864 *obj) {
    return func_800AFD78(obj->field_0, obj->count_4++);
}
