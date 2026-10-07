#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef struct {
    s16 delta;
    s16 index;
    u8 (*func)(void *self, s32 arg);
} VTableEntry;
typedef struct {
    char pad0[8];
    VTableEntry *vtable;
    s8 field_C;
} S;
s32 func_8010B9F4(S *s);
s32 func_8010CD1C(S *s) {
    s32 base = s->field_C;
    s16 bonus = func_8010B9F4(s);
    u8 plus = s->vtable[8].func((char *)s + s->vtable[8].delta, 0xB);
    u8 minus = s->vtable[8].func((char *)s + s->vtable[8].delta, 9);
    s32 total;
    s32 result;

    total = base + bonus + plus - minus;
    if (total < 0) {
        return 0;
    }
    if (total <= 0xFFFF) {
        result = total;
    } else {
        result = 0xFFFF;
    }
    return result;
}
