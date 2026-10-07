#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { u8 pad[8]; s32 f_8; u8 pad2[4]; } Item;
typedef struct { u8 pad[8]; u32 count; Item *items; } List;
typedef struct { u8 pad[0x38]; List list; } S;
static inline s32 list_size(List *l) {
    s32 total = 8;
    u32 i;
    for (i = 0; i < l->count; i++) {
        total += l->items[i].f_8 + 1;
    }
    return total;
}
s32 func_8006F958(S *s) { return list_size(&s->list); }
