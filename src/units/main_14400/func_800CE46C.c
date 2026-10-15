#include "common.h"
#include "collection_iterator.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

s32 func_800CE46C(void *list, s32 (*cb)(void *)) {
    CollectionIterator it;
    func_800CEB20(&it, list);
    while (func_800CEBA0(&it)) {
        if (cb(func_800CEC68(&it))) {
            return 1;
        }
    }
    return 0;
}
