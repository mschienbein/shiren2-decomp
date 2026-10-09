#include "common.h"

typedef unsigned char u8;
typedef struct { s32 field0; void *field4; } Iterator;
extern s32 func_800A9070(Iterator *iterator, s32 kind);
extern void *func_800A910C(Iterator *iterator);
extern u8 func_800A8C00(void *actor);

/* Returns the u8 value of the first actor of the given kind, 0 for kind 0x17,
 * or -1 when no such actor exists. One pointer local walks from the iterator
 * to the found actor. */
s32 func_80041C64(s32 kind)
{
    Iterator iterator;
    void *object = &iterator;
    s32 result = -1;

    iterator.field0 = 0;
    if (kind == 0x17) {
        result = 0;
    } else if (func_800A9070(object, kind)) {
        object = func_800A910C(object);
        result = func_800A8C00(object);
    }
    return result;
}
