#include "common.h"
#include "item_records.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;

/* Trap slot +0x5C supplies these three pointers; the default ignores them. */
s32 func_801123EC(void *self, void *actor, Item *item) {
    return 0;
}
