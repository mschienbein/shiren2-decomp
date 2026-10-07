#include "common.h"
typedef struct { void *x0; s32 x4; } Pair;
typedef struct {
    s32 count;
    s32 x4;
    s32 x8;
    s32 flags;
    s32 x10;
    void *x14;
    /* GNU C trailing payload: count relocation records follow the 0x18-byte header. */
    Pair entries[0];
} Hdr;
extern Hdr *D_801CA708;
void func_8012A910(Hdr *h) {
    s32 i;
    /* Before relocation the pointer slots encode byte offsets within this
     * variable-sized allocation; store live pointers after applying them. */
    unsigned char *base = (unsigned char *)h;
    if (h->flags & 1) return;
    if (D_801CA708 == 0) D_801CA708 = h;
    h->flags = 1;
    h->x10 = 0;
    h->x14 = base + (s32)h->x14;
    for (i = 0; i < h->count; i++) {
        h->entries[i].x0 = base + (s32)h->entries[i].x0;
    }
}
