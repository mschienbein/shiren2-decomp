#include "common.h"
#include "collection_iterator.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;

typedef struct { u8 kind; u8 pad1; u8 flags; } Entry800CF140;

s32 func_800CF140(void *list, u8 kind) {
    CollectionIterator it;
    s32 count = 0;

    func_800CEB20(&it, list);
    while (func_800CEBA0(&it)) {
        Entry800CF140 *e = func_800CEC68(&it);
        s32 match = 0;
        if (e->flags & 4) {
            match = e->kind == kind;
        }
        if (match) {
            count++;
        }
    }
    return count;
}
