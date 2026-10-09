#include "common.h"

typedef struct {
    unsigned char pad00[8];
    short adjust08;
    short pad0A;
    void (*destroy0C)(void *self, s32 flags);
} VTable;

typedef struct {
    unsigned char kind00;
    unsigned char pad01[7];
    VTable *vtable08;
} Item;

extern Item *func_800AACE0(s32 kind);
/* Destroy up to 100 leading kind-14 items; return the first survivor.
 * Both callers (func_800FA298, func_800FA894) pass their actor in a0; it is unused here. */
Item *func_800FA5FC(void *self) {
    Item *item = func_800AACE0(0);
    s32 remaining = 100;

    for (;;) {
        VTable *vtable;

        if (remaining-- <= 0 || item == 0 || item->kind00 != 14) {
            break;
        }
        vtable = item->vtable08;
        vtable->destroy0C((char *)item + vtable->adjust08, 3);
        item = func_800AACE0(0);
    }
    return item;
}
