#include "common.h"

typedef struct {
    void *collection; /* 0x00 */
    void *item;       /* 0x04 */
} Link;

extern s32 func_800CD090(void *container, void *element);

/* Clears the link's item when the collection rejects it. */
static inline void link_validate(Link *link) {
    if (link->collection == 0 || link->item == 0) return;
    if (func_800CD090(link->collection, link->item) < 0) link->item = 0;
}

void func_800D01B8(void *output, void *collection, void *item) {
    Link *link = output;

    link->collection = collection;
    link->item = item;
    link_validate(link);
}
