#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

typedef struct { void *field_0; void *field_4; } Entry800D01B8;
void func_800D01B8(Entry800D01B8 *entry, void *arg1, void *arg2);

typedef struct {
    u8 pad0[8];
    Entry800D01B8 *entries;
    s32 count;
    s32 index;
} Obj800D05E0;

void func_800D05E0(Obj800D05E0 *obj, void *arg1, void *arg2) {
    s32 i = obj->index;
    if (i < obj->count) {
        obj->index = i + 1;
        func_800D01B8(&obj->entries[i], arg1, arg2);
    }
}
